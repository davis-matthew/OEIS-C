/*
 * A173631  a(n) = ceiling(sqrt(4*P_n)), where P_n is product of first n
 *          primes.
 * https://oeis.org/A173631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(sqrt(4 * P_n)), where P_n is the product of the *
 *            first n primes. P_0 = 1, so a(0) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   floor_sqrt, seq_mul, seq_one, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a 128‑bit cache for the product of the first i
 * primes.  For seq_t sizes up to 8 bytes it works with __uint128_t.
 * For larger seq_t sizes (or arbitrary‑precision) it falls back to a
 * multiply‑by‑prime loop using the utilities from the repository. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../utils/sieve_first_n_primes.h" /* sieve_first_n_primes */
#include "../utils/seq_mul.h"              /* seq_mul */
#include "../utils/floor_sqrt.h"           /* floor_sqrt */
#include "../utils/seq_one.h"              /* seq_one */

/* 128‑bit unsigned type (available on C++ and GCC/Clang on 64‑bit targets) */
#if defined(__UINT128_T) || defined(__uint128_t) || defined(__SIZEOF_INT128__)
typedef unsigned __int128 uint128_t;
#else
/* If the compiler does not provide a native 128‑bit type the code will
   not compile; the 64‑bit build never needs such a type. */
#error "128‑bit type not available"
#endif

/* Cache of primes (0‑indexed) and the product of the first i primes
 * (product[0] = 1).  All indices are 0‑based; product[i] holds Π_{k=0}^{i-1} primes[k].
 */
static uint32_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;      /* number of cached primes          */

static uint128_t *prod_cache = NULL;
static size_t     prod_size  = 0;      /* length of prod_cache (≥1 if set) */

/* ------------------------------------------------------------------ */
/* Prime cache helper                                                */
/* ------------------------------------------------------------------ */
static void ensure_primes(size_t need)
{
    if (need <= prime_cnt) return;
    free(prime_cache);
    sieve_first_n_primes((idx_t)need, &prime_cache, &prime_cnt);
}

/* ------------------------------------------------------------------ */
/* 128‑bit product cache helper (product of first i primes)          */
/* ------------------------------------------------------------------ */
static void ensure_prod_cache(size_t need)
{
    /* need is the highest index we want (i.e. we need product[need]) */
    if (need + 1 <= prod_size) return;        /* already available */

    size_t old_size = prod_size;
    size_t new_size = need + 1;

    uint128_t *new_arr = (uint128_t *)calloc(new_size, sizeof(uint128_t));
    if (!new_arr) return;                     /* out of memory – leave cache as is */

    if (old_size > 0) {
        memcpy(new_arr, prod_cache, old_size * sizeof(uint128_t));
        free(prod_cache);
    }

    /* initialise the first element (product of zero primes) */
    if (old_size == 0) new_arr[0] = (uint128_t)1;

    /* ensure we have enough primes */
    ensure_primes(need);

    size_t start = (old_size > 0) ? old_size : 1;   /* indices already filled */
    for (size_t i = start; i <= need; ++i) {
        /* primes are 0‑indexed; the i‑th product includes primes[0..i-1] */
        new_arr[i] = new_arr[i - 1] * (uint128_t)prime_cache[i - 1];
    }

    prod_cache = new_arr;
    prod_size  = new_size;
}

/* ------------------------------------------------------------------ */
/* Integer square root of a 128‑bit unsigned integer                  */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_uint128(uint128_t x)
{
    uint64_t lo = 0, hi = (uint64_t)-1;   /* hi = 2^64 – 1 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint128_t mid2 = (uint128_t)mid * (uint128_t)mid;
        if (mid2 <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(x)) */
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* -------------------------------------------------------------- */
    /* 1.  Small seq_t types (≤ 64 bits).  Use the 128‑bit cache.     */
    /* -------------------------------------------------------------- */
    if (sizeof(seq_t) <= 8) {
        size_t idx = (size_t)n;
        ensure_prod_cache(idx);
        if (prod_cache == NULL || idx >= prod_size) return (seq_t)0;

        uint128_t prod = prod_cache[idx];
        /* 4 * prod must fit in 128 bits – it always does for the sizes we
           care about (otherwise the value itself would not fit in seq_t). */
        uint128_t val = prod * (uint128_t)4;

        uint64_t root = isqrt_uint128(val);
        if ((uint128_t)root * (uint128_t)root == val)
            return (seq_t)root;
        else
            return (seq_t)((uint64_t)root + 1);
    }

    /* -------------------------------------------------------------- */
    /* 2.  Larger seq_t types (more than 64 bits or arbitrary‑prec).  */
    /* -------------------------------------------------------------- */
    /* Build the product of the first n primes using seq_mul.         */
    seq_t current = seq_one();                 /* starts at 1 */

    if (n > 0) {
        uint32_t *primes = NULL;
        size_t primes_cnt = 0;
        sieve_first_n_primes((idx_t)n, &primes, &primes_cnt);
        for (size_t i = 0; i < primes_cnt; ++i) {
            current = seq_mul(current, (seq_t)primes[i]);
        }
        free(primes);
    }

    /* Compute 4 * P_n */
    seq_t val = seq_mul(current, (seq_t)4);

    /* Floor of sqrt(4 P_n) */
    seq_t t = floor_sqrt(val);
    seq_t t_sq = seq_mul(t, t);

    /* If it is a perfect square, return t; else return t+1 */
    if (t_sq == val)
        return t;
    else
        return t + (seq_t)1;
}
