/*
 * A024391  2nd elementary symmetric function of the first n+1 positive
 *          integers congruent to 2 mod 3.
 * https://oeis.org/A024391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3*n+2)*(3*n+5)/8
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; return 0 in that case */
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 1);
    res *= ((seq_t)3 * nn + 2);
    res *= ((seq_t)3 * nn + 5);
    res /= 8;
    return res;
}
