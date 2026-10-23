/*
 * A033265  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*2^i
 *          is the base-2 representation of n.
 * https://oeis.org/A033265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(log2(n)) - (# of 1→0 transitions in the binary
 *            representation of n)
 * Range:     u8 n <= 9569, u16 n <= 9569, u32 n <= 9569, u64 n <= 9569,
 *            u128 n <= 9569, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for unexpected 0 input */
    idx_t decreases = 0;
    idx_t i = 0;
    idx_t prev = n & 1ULL;                 /* bit 0 */
    for (idx_t t = n >> 1; t; t >>= 1) {
        idx_t cur = t & 1ULL;              /* next higher bit */
        if (prev == 1 && cur == 0)
            decreases++;
        prev = cur;
        i++;                               /* count of transitions examined */
    }
    return (seq_t)i - (seq_t)decreases;
}
