/*
 * A057361  a(n) = floor(5*n/8).
 * https://oeis.org/A057361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n/8)
 * Range:     u8 n <= 409, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A057361: a(n) = ⌊5*n/8⌋ for n ≥ 0. */
seq_t a(idx_t n)
{
    /* Perform the multiplication using a larger integer type to avoid
       overflow when seq_t is small (e.g., uint8_t or uint16_t). */
    unsigned long long prod = 5ULL * (unsigned long long)n;
    return (seq_t)(prod / 8ULL);
}
