/*
 * A085535  a(n) = (2n)^(2n-1).
 * https://oeis.org/A085535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)^(2n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 2n and exponent = 2n-1 */
    seq_t  base = (seq_t)n * ((seq_t)2);   /* base = 2*n  */
    idx_t  exp  = 2*n - 1;                 /* exp  = 2*n-1 */

    /* Binary exponentiation using only seq_t operations */
    seq_t  result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        if (exp > 1) {
            base *= base;
        }
        exp >>= 1;
    }
    return result;
}
