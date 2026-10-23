/*
 * A027338  Number of partitions of n that do not contain 4 as a part.
 * https://oeis.org/A027338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - A000041(n-4) (partitions of n with no part
 *            equal to 4)
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of partition numbers p[0..] */
    static seq_t *p = NULL;   /* dynamic array of partition counts   */
    static idx_t cur_max = 0; /* highest n that has been computed    */
    static size_t cap = 0;    /* current capacity of the array       */

    /* extend the array if necessary                          */
    size_t needed = (size_t)(n + 1);
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(p, needed * sizeof(seq_t));
        if (!tmp) exit(1); /* abort on allocation failure         */
        p   = tmp;
        cap = needed;
    }

    /* initialise p[0] on the very first call */
    if (cur_max == 0) {
        p[0] = (seq_t)1;
    }

    /* compute partition numbers up to n (pentagonal recurrence) */
    for (idx_t m = cur_max + 1; m <= n; ++m) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        int sign = 1;   /* +1 for the first generalized pentagonal number   */

        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;                 /* no more terms   */
            if (sign > 0) sum_pos += p[m - pent1];
            else           sum_neg += p[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= m) {
                if (sign > 0) sum_pos += p[m - pent2];
                else           sum_neg += p[m - pent2];
            }
            sign = -sign; /* alternate sign for next pair of terms  */
        }

        p[m] = sum_pos - sum_neg;  /* sum_pos ≥ sum_neg for all m          */
    }

    cur_max = n;

    /* result: partitions without a part equal to 4 */
    if (n < 4)
        return p[n];
    else
        return p[n] - p[n - 4];
}
