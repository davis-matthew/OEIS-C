/*
 * A047201  Numbers not divisible by 5.
 * https://oeis.org/A047201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + floor((n + 3) / 4)
 * Range:     u8 n <= 204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer that is not divisible by 5 (OEIS A047201). */
seq_t a(idx_t n)
{
    /* n ≥ 1; cast to seq_t to avoid narrowing when seq_t is larger than idx_t. */
    seq_t N = (seq_t)n;
    return (N - 1) + ((N + 3) / 4);
}
