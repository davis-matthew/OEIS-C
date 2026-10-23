/*
 * A037779  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,2,0.
 * https://oeis.org/A037779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = successive base‑4 digits 3,1,2,0 repeated. Hence a(n) =
 *            a(n‑1)*4 + d_n where d_n = {3,1,2,0}[ (n‑1) mod 4 ]. This is
 *            equivalent to interpreting the first n digits of the periodic
 *            sequence 3,1,2,0 as a base‑4 number
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* digits of the periodic pattern */
    static const int d[4] = {3, 1, 2, 0};

    for (idx_t i = 0; i < n; ++i)
    {
        /* i runs from 0, so the digit index is i mod 4 */
        res = res * (seq_t)4 + (seq_t)d[i & 3];
    }

    return res;
}
