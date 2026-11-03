/*
 * A128995  For a number n, let r=n+1, s = the least number greater than r and
 *          coprime to nr; n is here if nr-s, rs-n and sn-r are all prime.
 * https://oeis.org/A128995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   gcd, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128995.
 * For a number n, let r = n+1 and s be the least integer > r that
 * is coprime to n*r.  
 * n is a term of the sequence if the three numbers
 * n*r - s, r*s - n and s*n - r are all prime.
 *
 * This implementation searches for terms one by one, storing found
 * values in a dynamically growing cache.  It works for any supplied
 * seq_t and idx_t types. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_u64.h"

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

/* static cache of computed terms */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static seq_t next_candidate = 1;   /* next n to test */

/* ------------------------------------------------------------------ */
/* ensure that the cache contains at least "need" terms */
static void ensure_terms(idx_t need)
{
    while (cache_sz < need) {
        seq_t n = next_candidate++;          /* next integer to test   */
        seq_t r = n + (seq_t)1;              /* r = n + 1              */
        seq_t mr = n * r;                    /* n * r                  */

        /* find the least s > r coprime to mr */
        seq_t s = r + (seq_t)1;
        while (gcd(mr, s) != (seq_t)1) ++s;

        /* differences to test for primality */
        seq_t d1 = mr - s;        /* n*r - s */
        seq_t d2 = r * s - n;     /* r*s - n */
        seq_t d3 = s * n - r;     /* s*n - r */

        /* check that all are prime (>1) */
        if (d1 > (seq_t)1 && d2 > (seq_t)1 && d3 > (seq_t)1 &&
            is_prime_big(d1) && is_prime_big(d2) && is_prime_big(d3)) {
            /* grow cache if needed */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = n;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* undefined for index 0 */
        return (seq_t)0;
    ensure_terms(n);
    return cache[n - 1];
}
