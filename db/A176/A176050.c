/*
 * A176050  Det(M) where M is an n X n antisymmetric matrix with M(i,j) = n
 *          for i < j.
 * https://oeis.org/A176050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n if n is even, otherwise a(n) = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for odd n. */
    if (n & 1) {
        return (seq_t)0;
    }

    /* Compute n^n using exponentiation by squaring. */
    seq_t base = (seq_t)n;
    seq_t result = (seq_t)1;
    idx_t exp = n;          /* Keep original exponent */

    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return result;
}
