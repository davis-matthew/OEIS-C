/*
 * A121608  Primes that can be written as concatenation of two composite
 *          numbers in decimal representation.
 * https://oeis.org/A121608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9638 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A121608 - primes that can be written as concatenation of two composite numbers in decimal.
    The function computes terms by exhaustive search and memoises results.
    For each candidate integer, if it is prime and can be split into two
    decimal parts each >1 that are both composite, it is added to the cache.
    Subsequent calls continue from the last checked value.  */

#include <stdbool.h>
#include <stdlib.h>

/* Utility functions required by the implementation. */
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* ---------------------------------------------------------------------- */
/*   Helper: check whether a prime can be split into two composite parts  */
static bool is_valid_prime(seq_t p)
{
    int d = num_digits(p);                /* number of decimal digits */
    for (int s = 1; s < d; ++s) {
        seq_t div   = pow10((idx_t)(d - s));      /* 10^(d-s) */
        seq_t left  = p / div;
        seq_t right = p % div;
        if (left > 1 && right > 1 &&          /* both parts > 1       */
            !is_prime_big(left) &&           /* both parts composite*/
            !is_prime_big(right))
            return true;
    }
    return false;
}

/* ---------------------------------------------------------------------- */
/*   Cache management --------------------------------------------------- */
static seq_t *cache      = NULL; /* array holding found primes */
static size_t cache_len  = 0;    /* number of primes stored   */
static size_t cache_cap  = 0;    /* allocated capacity       */
static seq_t last_checked = 2;   /* candidate to test next    */

/* Ensure cache has room for at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;          /* double until sufficient */

    if (cache == NULL)
        cache = seq_alloc(new_cap);
    else
        cache = seq_realloc(cache, cache_cap, new_cap);
    if (!cache)
        abort();                /* allocation failure */

    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/*   Public sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset, harmless */

    size_t target = (size_t)n;      /* desired count of terms */

    while (cache_len < target) {
        /* Test next candidate for primality and validity */
        if (is_prime_big(last_checked) && is_valid_prime(last_checked)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
        last_checked++;           /* advance to next integer */
    }
    return cache[n - 1];
}
