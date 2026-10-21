/*
 * A063142  Dimension of the space of weight 2n cusp forms for Gamma_0( 74 ).
 * https://oeis.org/A063142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 for n = 1; * a(n) = ((-1)^n + 38*n - 23)/2 for n > 1 *
 *            This follows the closed‑form given in the OEIS entry for
 *            A063142. * The implementation uses only integer arithmetic and
 *            works for any * sequence element type seq_t
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special first term */
    if (n == 1) {
        return (seq_t)8;
    }

    /* Convert n to seq_t to ensure correct size of intermediate
       values (seq_t may be 8, 16, 32, 64 or 128 bits). */
    seq_t nseq = (seq_t)n;

    /* Compute 38*n - 23.  This fits in seq_t for all reasonable n. */
    seq_t temp = (seq_t)38 * nseq - (seq_t)23;

    /* Add or subtract 1 depending on the parity of n. */
    if ((n & 1) == 0) {            /* even n: (-1)^n = 1  */
        temp += (seq_t)1;
    } else {                       /* odd n:  (-1)^n = -1 */
        temp -= (seq_t)1;
    }

    /* Divide by 2 to obtain the result. */
    return temp / (seq_t)2;
}
