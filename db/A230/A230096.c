/*
 * A230096  Number of tilings of an n X 1 rectangle (using tiles of dimension
 *          1 X 1 and 2 X 1) that share no tile at the same position with
 *          their mirrored image.
 * https://oeis.org/A230096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 66, u64 n <= 130,
 *            u128 n <= 258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A230096: Number of tilings of an n×1 rectangle (using 1×1 and 2×1 tiles)
 * that share no tile in the same position with their mirrored image.
 *
 * Recurrence: a(0)=1, a(1)=0, a(2)=0, a(3)=2,
 *   a(n) = a(n-2)+2*a(n-4)   for n ≥ 4                          */

seq_t a(idx_t n)
{
    /* Handle the initial four values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;
    seq_t buf[4] = {(seq_t)1, (seq_t)0, (seq_t)0, (seq_t)2};

    if (n < 4) return buf[n];

    /* Iteratively compute a(n) using the 4‑term sliding window. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = buf[(i - 2) & 3] + (seq_t)2 * buf[(i - 4) & 3];
        buf[i & 3] = val;
    }
    return buf[n & 3];
}
