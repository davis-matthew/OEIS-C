/*
 * A180762  Half the number of nX2 binary arrays with each element equal to at
 *          least one neighbor.
 * https://oeis.org/A180762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-4) Initial values: a(1)=1,
 *            a(2)=3, a(3)=11, a(4)=34
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)34;

    /* last[0] = a(i-4), last[1] = a(i-3), last[2] = a(i-2), last[3] = a(i-1) */
    seq_t last[4];
    last[0] = (seq_t)1;
    last[1] = (seq_t)3;
    last[2] = (seq_t)11;
    last[3] = (seq_t)34;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = ((seq_t)3) * last[3] + last[2] - ((seq_t)2) * last[0];
        /* shift the window forward by one position */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = val;
    }
    return last[3];
}
