/*
 * A037621  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3.
 * https://oeis.org/A037621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + d_n, d_1=2, d_{k+1}= (d_k==2?0:(d_k==0?3:2)),
 *            with a(0) = 0. This generates the base‑7 number whose digits are
 *            the first n terms of the periodic sequence 2,0,3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;   /* a(0) = 0 */
    seq_t digit = 2;   /* first digit of the period */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 7 + digit;   /* shift base‑7 left and add new digit */
        /* advance to next digit in the 2‑0‑3 cycle */
        if (digit == 2)
            digit = 0;
        else if (digit == 0)
            digit = 3;
        else
            digit = 2;
    }
    return res;
}
