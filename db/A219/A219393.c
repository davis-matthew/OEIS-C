/*
 * A219393  Numbers k such that 23*k+1 is a square.
 * https://oeis.org/A219393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (46*n*(n-1) + 19*(-1)^n*(2*n - 1) + 3) / 8 + 2 (OEIS
 *            A219393)
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 is a special case (negative intermediate would occur otherwise) */
    if (n == 1) return (seq_t)0;

    /* Convert n to seq_t for arithmetic */
    seq_t n_t = (seq_t)n;

    /* Basic products */
    seq_t term1 = (seq_t)46 * n_t * (n_t - 1);
    seq_t term2 = (seq_t)19 * ( (seq_t)(2 * n_t - 1) );

    /* Compute numerator according to parity of n */
    seq_t sum;
    if ((n & 1) == 0) {                 /* even n */
        sum = term1 + term2 + 3;
    } else {                            /* odd n (n ≥ 3 for this branch) */
        sum = term1 - term2 + 3;
    }

    /* Final value */
    seq_t result = sum / 8 + 2;
    return result;
}
