/*
 * A116521  Binomial transform of tetranacci sequence A000078.
 * https://oeis.org/A116521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 8*a(n-2) + 6*a(n-3) - a(n-4)
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;

    /* initialise the first four terms if the cache is empty */
    if (seq_len == 0) {
        seq_len = 4;
        seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)0;
        seq[2] = (seq_t)0;
        seq[3] = (seq_t)1;
    }

    /* if the requested index is already computed, return it */
    if (n < seq_len)
        return seq[n];

    /* remember the length before possible reallocation   */
    idx_t old_len = seq_len;

    /* enlarge the cache to hold indices up to n */
    if (n + 1 > seq_len) {
        seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
        seq_len = n + 1;
    }

    /* compute the missing terms using the recurrence */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t t = (seq_t)5 * seq[i - 1];
        t -= (seq_t)8 * seq[i - 2];
        t += (seq_t)6 * seq[i - 3];
        t -= seq[i - 4];
        seq[i] = t;
    }

    return seq[n];
}
