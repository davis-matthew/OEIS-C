/*
 * A141850  Primes congruent to 3 mod 11.
 * https://oeis.org/A141850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes congruent to 3 modulo 22 (OEIS A141850)
 * Range:     u8 n <= 6, u16 n <= 487, u32 n <= 624, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static seq_t last = 0;   /* last prime meeting the condition */

    /* initialize cache */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* first term is 3 */
        cache[0] = (seq_t)3;
        sz = 1;
        last = (seq_t)3;
    }

    /* generate further terms until we have at least n terms */
    while (sz < n) {
        seq_t cand = last + 2;      /* start with next odd candidate */
        for (;;) {
            if (is_prime_big(cand) && ((cand % (seq_t)22) == (seq_t)3)) {
                if (sz >= cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[sz++] = cand;
                last = cand;
                break;
            }
            cand += 2;              /* next odd number */
        }
    }

    return cache[n - 1];
}
