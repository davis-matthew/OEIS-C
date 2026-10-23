/*
 * A168202  a(n) = 3*n - a(n-1) + 1 with n > 1, a(1)=5.
 * https://oeis.org/A168202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 - 9*(-1)^n)/4 + 3*n/2 Equivalent integer form used in
 *            code: a(n) = (6*n + 14)/4 for odd n a(n) = (6*n - 4)/4 for even
 *            n (For n >= 1)
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1 by caller; behaviour for n == 0 is undefined */
    seq_t nseq = (seq_t)n;   /* cast index to seq_t for careful arithmetic */

    /* Use unsigned arithmetic; adjust numerator according to parity of n */
    seq_t numerator;
    if ((n & 1) == 0)           /* n even */
        numerator = 6 * nseq - 4;
    else                        /* n odd */
        numerator = 6 * nseq + 14;

    /* The numerator is always divisible by 4, so integer division is exact */
    return numerator / 4;
}
