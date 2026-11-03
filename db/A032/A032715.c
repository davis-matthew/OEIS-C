/*
 * A032715  Numbers k such that k prefixed by '7' and followed by '3' is
 *          prime.
 * https://oeis.org/A032715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032715: numbers k such that 7 followed by k then 3 is prime. */
/* The value a(n) is the n‑th such integer, 1‑based.  The function uses a
 * simple incremental search with memoisation; it is correct for all n
 * provided the auxiliary functions compile under C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
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

/* Public API ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Cache of already discovered terms (k values). */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Next candidate k to test. */
    static idx_t current_k   = 1;

    /* Allocate initial cache if this is the first call. */
    if (cache == NULL) {
        cache_cap = 64;
        /* Use a C‑style cast so the code compiles as both C and C++. */
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t k = current_k++;                /* candidate k */
        int d   = num_digits_idx(k);                  /* decimal digits of k */

        /* Construct N = 7 * 10^(d+1) + k*10 + 3 as a seq_t. */
        seq_t p = pow10((idx_t)(d + 1));      /* 10^(d+1) */
        seq_t N = 7 * p + ((seq_t)k * 10 + 3);

        /* Test primality. */
        if (is_prime_big(N)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) return (seq_t)0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* Return the n‑th term (1‑based). */
    return cache[(size_t)n - 1];
}
