/*
 * A133401  Diagonal of polygorial array T(n,k) = n-th polygorial for k = n,
 *          for n > 2.
 * https://oeis.org/A133401
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = \prod_{i=1}^{n}\frac{(n-2)\,i^2-(n-4)\,i}{2}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (the diagonal of the polygorial array).  */
/*  a(n) is defined for n >= 3; for n < 3 the function returns 0.  */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t result = 1;
    seq_t sn = (seq_t)n;          /* cast once for efficiency */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii  = (seq_t)i;
        /* compute ((n-2)*i*i - (n-4)*i) / 2  */
        seq_t term = (sn - 2) * ii * ii;
        term -= (sn - 4) * ii;
        term /= 2;
        result *= term;
    }

    return result;
}
