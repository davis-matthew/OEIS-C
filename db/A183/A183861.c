/*
 * A183861  a(1) = 1; for n > 1, a(n) = n - 1 + ceiling((n^2 - 1)/3);
 *          complement of A183860.
 * https://oeis.org/A183861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceil((n^2 - 1)/3) for n > 1; a(1) = 1
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    seq_t nn = (seq_t)n;
    /* ceil((n^2 - 1)/3) equals (n^2 + 1)/3 for integer n > 1 */
    seq_t res = (nn * nn + (seq_t)1) / (seq_t)3;
    res += nn - (seq_t)1;
    return res;
}
