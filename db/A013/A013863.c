/*
 * A013863  a(n) = 12^(5*n + 2).
 * https://oeis.org/A013863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(5*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent is an unsigned 64‑bit integer so it does not overflow for
       the values of n that are relevant to the sequence.  The base and
       result stay in seq_t. */
    idx_t exp = n * 5 + 2;
    seq_t base = 12;
    seq_t result = 1;

    /* fast exponentiation by squaring */
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}
