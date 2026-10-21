/*
 * A060722  a(n) = 3^(n^2).
 * https://oeis.org/A060722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the exponent n^2.  For n up to 10000 this fits in 64‑bit. */
    idx_t exp = n * n;

    /* Repeated squaring to compute 3^exp.  All arithmetic is done in seq_t. */
    seq_t result = 1;
    seq_t base   = 3;

    while (exp > 0) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
