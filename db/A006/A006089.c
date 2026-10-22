/*
 * A006089  Coefficients of elliptic function cn.
 * https://oeis.org/A006089
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25^(n+1) - 8*n*9^(n+1) + 32*n^2 + 16*n - 25)/256,
 *            computed via the equivalent linear recurrence for b(j) = a(j-1):
 *            b(j) = 46*b(j-1) - 663*b(j-2) + 3748*b(j-3) - 7711*b(j-4) +
 *            6606*b(j-5) - 2025*b(j-6), b(0) = b(1) = b(2) = 0, b(3) = 16,
 *            b(4) = 912, b(5) = 30768 (characteristic polynomial...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t j = n + 1;
    idx_t i;
    seq_t b0 = 0, b1 = 0, b2 = 0, b3 = 16, b4 = 912, b5 = 30768;
    seq_t pos, neg, nb;

    if (j <= 2) return (seq_t)0;
    if (j == 3) return b3;
    if (j == 4) return b4;
    if (j == 5) return b5;

    for (i = 6; i <= j; i++) {
        pos = b5 * 46;
        pos += b3 * 3748;
        pos += b1 * 6606;
        neg = b4 * 663;
        neg += b2 * 7711;
        neg += b0 * 2025;
        nb = pos - neg;
        b0 = b1;
        b1 = b2;
        b2 = b3;
        b3 = b4;
        b4 = b5;
        b5 = nb;
    }
    return b5;
}
