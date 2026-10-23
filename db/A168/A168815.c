/*
 * A168815  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A168815 – number of reduced words of length n in a Coxeter
 * group on 42 generators S_i with (S_i)^2 = (S_iS_j)^19 = I.
 *
 * Recurrence (linear of order 19):
 *   a(n) = 40·∑_{i=1}^{18} a(n-i) – 820·a(n-19) + P(n)
 * where P(0)=1, P(1..18)=2, P(19)=1, P(n)=0 for n>19.
 *
 * The function memoises computed values so that arbitrary order of
 * calls is supported.  All arithmetic is carried out in the type
 * seq_t, which may be 8, 16, 32, 64, 128 bits or a big‑integer class.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised values of a(n)              */
static size_t cache_cap = 0;    /* number of elements currently valid   */

extern seq_t a(idx_t n)
{
    /* Extend cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failure */
        cache = new_cache;

        /* Compute new terms sequentially */
        for (size_t k = cache_cap; k < new_cap; ++k) {
            seq_t sum18 = (seq_t)0;
            for (size_t i = 1; i <= 18 && k >= i; ++i)
                sum18 += cache[k - i];

            seq_t val = (seq_t)40 * sum18;
            if (k >= 19)
                val -= (seq_t)820 * cache[k - 19];

            seq_t pk = (seq_t)0;
            if (k == 0)          pk = (seq_t)1;
            else if (k <= 18)    pk = (seq_t)2;
            else if (k == 19)    pk = (seq_t)1;

            cache[k] = val + pk;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
