/*
 * A186866  Number of 7-step king's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 89928*n^2 - 555464*n + 817760 for n>5 Exact values are
 *            used for n <= 5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exact values for n = 1..5 */
    static const seq_t small[] = {0, 0, 0, 2984, 68272, 296360};

    if (n <= 5)
        return small[n];

    seq_t sn = (seq_t)n;
    /* compute 89928*n^2 - 555464*n + 817760 */
    seq_t val = (seq_t)89928 * sn * sn
              - (seq_t)555464 * sn
              + (seq_t)817760;
    return val;
}
