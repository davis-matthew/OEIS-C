/*
 * A117682  Prime numbers for which the product of the digits is a triangular
 *          number.
 * https://oeis.org/A117682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime for which the product of its decimal
 *            digits is a triangular number. The sequence begins with a(1)=3,
 *            a(2)=11,
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9944 (2026-11-03)
 * Helpers:   digit_product, ipow_i32, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return true iff m is a triangular number: m = k*(k+1)/2 for some integer k. */
static inline bool is_triangular(seq_t m)
{
    seq_t t = m * (seq_t)8 + (seq_t)1;            /* t = 8*m + 1          */
    seq_t s = floor_sqrt(t);                     /* s = floor(sqrt(t))   */
    seq_t sq = s * s;                            /* sq = s^2             */
    return sq == t;                              /* check perfect square */
}

/* --------------------------------------------------------------------- */
/* The main sequence function.  It lazily expands a static cache until
   the requested index is reached.  The cache is independent of the call
   order, so a(n) can be requested in any sequence.                      */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not part of OEIS, but safe for out‑of‑range calls */

    /* static cache of results */
    static seq_t *cache = NULL;              /* pointer to array of primes */
    static size_t cap = 0;                   /* current capacity of cache   */
    static size_t cnt = 0;                   /* number of cached terms      */
    static uint64_t last_prime = 1;          /* prime to start searching from */

    /* grow cache until we have at least n terms */
    while (cnt < n) {
        /* get next prime */
        last_prime = next_prime(last_prime);

        /* compute product of its decimal digits */
        seq_t prod = digit_product((seq_t)last_prime);

        /* keep this prime if the product is triangular */
        if (is_triangular(prod)) {
            if (cnt == cap) {                /* enlarge cache if necessary */
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();           /* allocation failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[cnt++] = (seq_t)last_prime;
        }
    }

    return cache[n - 1];
}
