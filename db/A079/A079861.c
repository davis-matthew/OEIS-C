/*
 * A079861  a(n) is the number of occurrences of 7's in the palindromic
 *          compositions of 2*n-1, or also, the number of occurrences of 8's
 *          in the palindromic compositions of 2*n.
 * https://oeis.org/A079861
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2) * 2^(n-8) for n >= 8 (offset 8)
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 34, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;            /* value undefined below offset */

    /* compute 2^(n-8) as a left shift of 1 */
    seq_t two_pow = ((seq_t)1) << (n - 8);

    /* multiply by (n+2), casting the index to seq_t */
    return ((seq_t)(n + 2)) * two_pow;
}
