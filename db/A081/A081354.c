/*
 * A081354  Numbers k such that sigma(k^2) modulo 4 = 3.
 * https://oeis.org/A081354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that sigma(k²) ≡ 3 (mod
 *            4). Equivalently: let r = #{ primes p|k : (p=2 and v₂(k)>0) or
 *            (p≡1(mod4) and v_p(k) odd) }; k belongs to the sequence iff r is
 *            odd
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include only required standard headers. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper to test whether a number k has an odd number of
   prime factors of the types described above.  k fits in uint64_t
   for the required range. */
static bool is_valid_uint64(uint64_t k)
{
    size_t r = 0;                    /* count of contributing primes */

    /* Handle factor 2 */
    if ((k & 1ULL) == 0) {
        r++;                         /* exponent of 2 ≥ 1 contributes 3 mod 4 */
        while ((k & 1ULL) == 0)
            k >>= 1;
    }

    /* Odd prime factors */
    for (uint64_t d = 3; (uint64_t)d * d <= k; d += 2) {
        if (k % d == 0) {
            size_t exp = 0;
            while (k % d == 0) {
                k /= d;
                exp++;
            }
            if ((d % 4ULL) == 1ULL && (exp & 1ULL))
                r++;                 /* p ≡ 1 (mod 4) and exponent odd */
        }
    }

    /* Remaining prime factor if any */
    if (k > 1) {
        if ((k % 4ULL) == 1ULL)      /* exponent is 1 (odd) */
            r++;
    }

    return (r & 1U) != 0;            /* r odd? */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t next_candidate = 1;  /* next integer to test */

    /* Grow cache if necessary. */
    if (cache == NULL) {
        size_t init_cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        cache_size = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Ensure cache capacity. */
        if (cache_size + 1 > (size_t)sizeof(cache)/sizeof(seq_t)) {
            size_t new_cap = (cache_size * 2) + 16;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        }

        /* Test next_candidate for validity. */
        if (is_valid_uint64((uint64_t)next_candidate)) {
            cache[cache_size] = (seq_t)next_candidate;
            cache_size++;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
