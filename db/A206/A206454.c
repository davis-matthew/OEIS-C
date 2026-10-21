/*
 * A206454  Number of 0..7 arrays of length n avoiding the consecutive pattern
 *          0..7.
 * https://oeis.org/A206454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-8) for n ≥ 8; a(0) = 1, a(k) = 8^k for 1 ≤
 *            k ≤ 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases first. */
    if (n == 0ul)
        return (seq_t)1;

    if (n < 8ul) {
        seq_t res = (seq_t)1;
        for (idx_t i = 0ul; i < n; ++i)
            res = res * (seq_t)8;
        return res;
    }

    /* Ring buffer holding the last 8 values of the sequence. */
    seq_t ring[8];
    ring[0] = (seq_t)1;               /* a(0) */
    for (idx_t i = 1ul; i < 8ul; ++i)  /* a(1) … a(7) = 8^i */
        ring[i] = ring[i - 1] * (seq_t)8;

    /* Iterate from a(8) up to a(n). */
    for (idx_t i = 8ul; i <= n; ++i) {
        seq_t val = ring[(i - 1) % 8ul] * (seq_t)8 - ring[i % 8ul];
        ring[i % 8ul] = val;
    }

    return ring[n % 8ul];
}
