/*
 * A143337  Expansion of K(k) * (6 * E(k) - (1 + 4*k'^2) * K(k)) / (Pi/2)^2 in
 *          powers of q where E(k), K(k) are complete elliptic integrals and q
 *          = exp(-Pi * K(k') / K(k)).
 * https://oeis.org/A143337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A143337(n) = 1 if n==0, else (-1)^{n+1} * 24 * sigma_1(n) where
 *            sigma_1(n) = sum of all positive divisors of n
 * Range:     i8 n <= 3, i16 n <= 47, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* a(0) = 1 */

    seq_t sigma = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i) sigma += (seq_t)j;
        }
    }

    seq_t res = (seq_t)24 * sigma;
    if ((n & 1) == 0)    /* n even => negative sign */
        res = -res;
    return res;          /* for odd n, sign is positive */
}
