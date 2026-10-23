/*
 * A046818  Number of 1's in binary expansion of 3n+1.
 * https://oeis.org/A046818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(3n+1)
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of 1's in the binary expansion of 3*n+1 */

seq_t a(idx_t n)
{
    /* convert index to seq_t first to avoid overflow in intermediate ops */
    seq_t sqn = (seq_t)n;
    seq_t m = sqn * 3 + 1;          /* 3*n + 1 */

    seq_t count = 0;
    seq_t x = m;

    while (x) {
        if (x & 1) {
            count = count + 1;
        }
        x = x >> 1;
    }

    return count;
}
