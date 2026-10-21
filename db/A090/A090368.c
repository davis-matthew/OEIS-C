/*
 * A090368  a(1) = 1; for n > 1, smallest divisor > 1 of 2n-1.
 * https://oeis.org/A090368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (if n==1); otherwise the smallest divisor greater than
 *            1 of (2*n-1)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial division; for odd numbers the first odd divisor is tested in increasing order. */
seq_t a(idx_t n)
{
    /* n is uint64_t; compute 2*n-1, cast to seq_t to avoid overflow before multiplication. */
    seq_t m = ((seq_t)n) * 2 - 1;
    if (n == 1) return 1;
    /* special case: if m is 1 (never occurs for n>1) return 1 */
    if (m <= 1) return 1;
    /* trial division: 2 cannot divide m because m is odd. Start from 3. */
    for (seq_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) return d;
    }
    /* m is prime */
    return m;
}
