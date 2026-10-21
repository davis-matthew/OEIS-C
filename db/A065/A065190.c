/*
 * A065190  Self-inverse permutation of the positive integers: 1 is fixed,
 *          followed by an infinite number of adjacent transpositions (n n+1).
 * https://oeis.org/A065190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (-1)^n for n>1, a(1) = 1
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; for n even a(n) = n + 1, for n odd a(n) = n – 1 */
seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)1;
    if ((n & 1U) == 0U)
        return (seq_t)n + (seq_t)1;
    else
        return (seq_t)n - (seq_t)1;
}
