/*
 * A090773  Numbers that are congruent to {4, 6} mod 10.
 * https://oeis.org/A090773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*((n+1)/2 - 1) + (n%2 ? 4 : 6) (for n ≥ 1)
 * Range:     u8 n <= 51, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the sequence of positive integers congruent to 4 or 6 modulo 10. */
seq_t a(idx_t n)
{
    /* Compute the pair index k = ceil(n/2) = (n+1)/2 */
    seq_t k = ((seq_t)n + 1u) / 2u;
    seq_t base = 10u * (k - 1u);          /* 10*(k-1) */
    seq_t offset = (n & 1u) ? 4u : 6u;    /* 4 if n odd, 6 if n even */
    return base + offset;
}
