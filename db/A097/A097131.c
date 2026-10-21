/*
 * A097131  F(n)+(-1)^n*F(n-1).
 * https://oeis.org/A097131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n‑2) – a(n‑4) for n ≥ 4 with a(0) = 1, a(1) = 1, a(2)
 *            = 2, a(3) = 1 The implementation is O(n) with no dynamic
 *            allocation and works for any seq_t (integral or
 *            arbitrary‑precision)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for 0 ≤ i < 4 */
    seq_t buf[4] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1};

    /* Return directly if n is a base index */
    if (n < 4)
        return buf[n];

    /* Compute successive terms up to n,
       keeping only the last 4 necessary values. */
    for (idx_t i = 4; i <= n; ++i) {
        /* buf[(i‑2)&3] is a(i‑2), buf[(i‑4)&3] is a(i‑4) */
        seq_t next = (seq_t)3 * buf[(i - 2) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = next;            /* store a(i) */
    }

    return buf[n & 3];
}
