/*
 * A047419  Numbers that are congruent to {1, 2, 3, 4, 6} mod 8.
 * https://oeis.org/A047419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n+4)/5) - d, where d = {2,7,6,5,4}[n mod 5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the quotient k = floor((n+4)/5). */
    seq_t k = ((seq_t)(n + 4)) / 5;

    /* 8 * k. */
    seq_t base = k * 8;

    /* n mod 5. */
    idx_t r = n % 5;

    /* Subtraction constants. */
    static const unsigned int sub[5] = {2, 7, 6, 5, 4};

    return base - (seq_t)sub[(unsigned int)r];
}
