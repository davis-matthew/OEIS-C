/*
 * A174644  Number of ways to place 5 nonattacking amazons (superqueens) on a
 *          5 X n board.
 * https://oeis.org/A174644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5-44 n^4+823 n^3-8168 n^2+42796 n-93984 for n>=12 (the
 *            initial values are hard‑coded for n<12)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 93, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for 1 <= n < 12 */
    static const seq_t tbl[11] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)0, (seq_t)0, (seq_t)4, (seq_t)40, (seq_t)204, (seq_t)708
    };

    if (n < 12)
        return tbl[n - 1];

    /* compute polynomial for n >= 12 */
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;            /* n^2 */
    seq_t x3 = x2 * x;           /* n^3 */
    seq_t x4 = x3 * x;           /* n^4 */
    seq_t x5 = x4 * x;           /* n^5 */

    seq_t pos = x5
              + (seq_t)823 * x3
              + (seq_t)42796 * x;

    seq_t neg = (seq_t)44 * x4
              + (seq_t)8168 * x2
              + (seq_t)93984;

    return pos - neg;
}
