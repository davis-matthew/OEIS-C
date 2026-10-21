/*
 * A054843  Number of sequences of consecutive nonnegative integers (including
 *          sequences of length 1) that sum to n.
 * https://oeis.org/A054843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{k ≥ 1 : n - k(k-1)/2 is divisible by k and ≥ 0}
 * Range:     u8 n <= 943, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, the number of ways to write n as a sum of consecutive
   non‑negative integers (including the trivial sequence of length 1). */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* Compute the triangular number k*(k-1)/2 using 128‑bit to avoid overflow */
        unsigned __int128 tri128 = (unsigned __int128)k * (k - 1) / 2;
        if (tri128 > (unsigned __int128)n)           /* k is too large, stop */
            break;

        /* Convert to seq_t; it fits because tri128 ≤ n and n can be cast to seq_t */
        seq_t triangular = (seq_t)tri128;

        /* Remaining value that must be divisible by k */
        seq_t rem = (seq_t)n - triangular;
        if (rem % (seq_t)k == 0)
            ++count;
    }
    return count;
}
