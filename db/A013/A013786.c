/*
 * A013786  a(n) = 7^(4*n + 1).
 * https://oeis.org/A013786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 7 raised to an arbitrary exponent using binary
 * exponentiation. This works for all supported seq_t sizes
 * without requiring a separate power routine. */

seq_t a(idx_t n)
{
    /* exponent e = 4*n + 1 (fits into idx_t for all practical n) */
    idx_t e = 4 * n + 1;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)7;

    while (e) {
        if (e & (idx_t)1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}
