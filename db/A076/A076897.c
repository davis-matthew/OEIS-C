/*
 * A076897  a(1)=1, a(n)=n-a(floor(3n/4)).
 * https://oeis.org/A076897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(floor(3n/4)), a(1)=1
 * Range:     u8 n <= 446, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] is never used for the recurrence, it only
       protects the "n==0" case that is out of the official
       index range of the sequence.  */
    static seq_t *cache = NULL;      /* cache[i] holds a(i) for i <= max_computed */
    static idx_t max_computed = 0;   /* largest index that has been filled */

    if (n == 0)
        return 0;                    /* undefined for n<1 – signal error safely */

    if (n > max_computed) {
        size_t new_len = (size_t)(n + 1);             /* need indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)             /* allocation failure */
            return 0;
        cache = tmp;

        /* initialise the sentinel cache[0] only once */
        if (max_computed == 0)
            cache[0] = 0;

        /* compute the new terms in increasing order */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t m = (i * 3) / 4;        /* floor(3i/4) */
            cache[i] = (seq_t)i - cache[m];
        }
        max_computed = n;
    }
    return cache[n];
}
