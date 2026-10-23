/*
 * A117030  a(1) = a(2) = 1; a(n) = a(n-1)*a(n-2) - a(n-3) - a(n-4) - ... -
 *          a(1) for n>2.
 * https://oeis.org/A117030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 11, i32 n <= 12, i64 n <= 14, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=1; 
   a(n)=a(n-1)*a(n-2) - (a(n-3)+a(n-4)+...+a(1))   for n>2   */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* memoised arrays a[1..computed], sum[0..computed] */
    static seq_t  *seq  = NULL;   /* values   */
    static seq_t  *sum  = NULL;   /* cumulative sums    */
    static idx_t  computed = 0;   /* highest index computed */
    static size_t cap = 0;        /* allocated capacity */

    /* increase capacity if necessary */
    if (n + 1 > cap) {
        size_t newcap = (cap == 0) ? 16 : cap * 2;
        if (newcap < (size_t)(n + 1))
            newcap = n + 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        sum = (seq_t *)realloc(sum, newcap * sizeof(seq_t));
        /* initialise base of sum array */
        if (computed == 0)
            sum[0] = 0;
        cap = newcap;
    }

    /* compute terms up to n */
    while (computed < n) {
        idx_t i = computed + 1;   /* next index to compute */

        if (i == 1 || i == 2) {
            seq[i] = 1;
            sum[i] = sum[i-1] + seq[i];
        } else {
            seq[i] = seq[i-1] * seq[i-2] - sum[i-3];
            sum[i] = sum[i-1] + seq[i];
        }
        computed = i;
    }

    return seq[n];
}
