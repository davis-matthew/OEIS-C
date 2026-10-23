/*
 * A143281  Number of binary words of length n containing at least one subword
 *          101 and no subword 11.
 * https://oeis.org/A143281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A143281: Number of binary words of length n containing at least one
 *  subword 101 and no subword 11.
 *
 *  Recurrence (for n ≥ 5):
 *      a(n) = 2*a(n-1) - a(n-4) - a(n-5)
 *  Initial values:
 *      a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=2
 *
 *  The function keeps a static cache so that each n can be queried in
 *  O(n) time and from any call order.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and its current size */
    static seq_t *cache      = NULL;
    static idx_t computed_sz = 0;   /* number of values already computed */

    /* need to grow cache up to index n */
    if (n + 1 > computed_sz) {
        /* grow array: realloc works when cache is NULL */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;

        /* compute new values */
        for (idx_t i = computed_sz; i <= n; ++i) {
            if (i == 0 || i == 1 || i == 2) {
                cache[i] = (seq_t)0;
            } else if (i == 3) {
                cache[i] = (seq_t)1;
            } else if (i == 4) {
                cache[i] = (seq_t)2;
            } else {
                /* recurrence: a(i) = 2*a(i-1) - a(i-4) - a(i-5) */
                seq_t val = cache[i - 1] * ((seq_t)2);
                val -= cache[i - 4];
                val -= cache[i - 5];
                cache[i] = val;
            }
        }
        computed_sz = n + 1;
    }

    return cache[n];
}
