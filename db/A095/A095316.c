/*
 * A095316  Primes in whose binary expansion the number of 1-bits is > number
 *          of 0-bits minus 2.
 * https://oeis.org/A095316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 979 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of those primes p whose binary expansion satisfies
   popcount(p) > (bitlen(p) - popcount(p) - 2).  For a prime p with
   bitlen(p)=len and popcount(p)=pop this is equivalent to
   2*pop > len-2  <=>  2*pop >= len-1.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_length.h"
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of accepted primes */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of stored primes */
    static seq_t last_cand = 1;      /* last candidate examined */

    /* Extend the cache until it contains at least n elements. */
    while (cache_len < n) {
        seq_t cand = last_cand + 1;          /* next integer to test */
        last_cand = cand;                    /* remember it for the next call */

        if (!is_prime_big(cand))
            continue;                       /* skip non‑primes */

        /* Compute popcount. */
        seq_t x = cand;
        seq_t pop = 0;
        while (x) {
            pop += (x & 1);
            x >>= 1;
        }

        /* Length of the binary representation. */
        unsigned len = bit_length(cand);

        /* Check the inequality 2*pop >= len-1. */
        if ((pop << 1) >= (seq_t)(len - 1)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
