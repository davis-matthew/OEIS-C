/*
 * A075818  Even numbers with exactly 3 prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A075818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A001358(n) (even numbers with exactly three prime
 *            factors)
 * Range:     u8 n <= 42, u16 n <= 2626, u32 n <= 2626, u64 n <= 2626,
 *            u128 n <= 2626, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                      /* undefined for n=0, return 0 */

    /* static cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;                  /* number of stored terms */
    static size_t cap   = 0;                  /* capacity of the cache */
    static uint64_t last = 7;                 /* last integer examined (next will be 8) */

    /* generate terms until we have n of them */
    if (len < (size_t)n) {
        /* allocate initial cache if necessary */
        if (cap == 0) {
            cap = 128;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache)
                abort();
        }

        while (len < (size_t)n) {
            uint64_t cand = ++last;            /* next candidate integer */

            /* only even numbers can have 3 prime factors in this sequence */
            if ((cand & 1ull) == 0ull) {
                /* n has exactly three prime factors (with multiplicity) ? */
                if (count_prime_factors((seq_t)cand) == (seq_t)3) {
                    /* enlarge cache if necessary */
                    if (len >= cap) {
                        cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                        if (!tmp)
                            abort();
                        cache = tmp;
                    }
                    cache[len++] = (seq_t)cand;   /* store the term */
                }
            }
        }
    }

    return cache[n - 1];
}
