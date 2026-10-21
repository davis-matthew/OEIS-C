/*
 * A065621  Reversing binary representation of n. Converting sum of powers of
 *          2 in binary representation of a(n) to alternating sum gives n.
 * https://oeis.org/A065621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = n XOR ((n - lowbit(n)) << 1) where lowbit(n) = n & (-n)
 *            (the lowest set bit of n)
 * Range:     u8 n <= 128, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote to seq_t for arithmetic. */
    seq_t n_t = (seq_t)n;

    /* Compute lowest set bit in n as unsigned two's‑complement. */
    idx_t low = n & -n;                /* low is a power of two. */
    seq_t low_t = (seq_t)low;

    /* a(n) = n XOR (2*(n - lowbit)) = n XOR ((n - lowbit) << 1). */
    return n_t ^ ((n_t - low_t) << 1);
}
