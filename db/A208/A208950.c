/*
 * A208950  a(4*n) = n*(16*n^2-1)/3, a(2*n+1) = n*(n+1)*(2*n+1)/6, a(4*n+2) =
 *          (4*n+1)*(4*n+2)*(4*n+3)/6.
 * https://oeis.org/A208950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*n) = n*(16*n^2-1)/3 * a(2*n+1) = n*(n+1)*(2*n+1)/6 *
 *            a(4*n+2) = (4*n+1)*(4*n+2)*(4*n+3)/6
 * Range:     u8 n <= 13, u16 n <= 73, u32 n <= 1797, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res;
    idx_t r = n % 4;
    if (r == 0) {
        seq_t k = n / 4;
        seq_t temp = k * k;
        temp = temp * 16;
        res = k * (temp - 1) / 3;
    } else if (r == 2) {
        seq_t t = (n - 2) / 4;
        seq_t a1 = 4 * t + 1;
        seq_t a2 = 4 * t + 2;
        seq_t a3 = 4 * t + 3;
        res = a1 * a2 * a3 / 6;
    } else { /* odd indices */
        seq_t t = (n - 1) / 2;
        res = t * (t + 1) * (2 * t + 1) / 6;
    }
    return res;
}
