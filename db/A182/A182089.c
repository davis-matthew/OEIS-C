/*
 * A182089  Numbers of the form (330*k+7)*(660*k+13)*(990*k+19)*(1980*k+37).
 * https://oeis.org/A182089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (330*(n-1)+7)*(660*(n-1)+13)*(990*(n-1)+19)*(1980*(n-1)+37)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 82, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k    = (seq_t)n - 1;          /* k = n-1  (offset 1) */
    seq_t t1   = 330 * k + 7;
    seq_t t2   = 660 * k + 13;
    seq_t t3   = 990 * k + 19;
    seq_t t4   = 1980 * k + 37;
    return t1 * t2 * t3 * t4;
}
