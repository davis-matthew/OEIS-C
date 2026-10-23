/*
 * A118173  Decimal representation of n-th iteration of the Rule 188
 *          elementary cellular automaton starting with a single black cell.
 * https://oeis.org/A118173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2) + 16*a(n-4) - 16*a(n-6) for n > 5 Initial values:
 *            a(0)=1, a(1)=3, a(2)=5, a(3)=15, a(4)=29, a(5)=55
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return any of the first six terms directly */
    static const seq_t init[6] = {1, 3, 5, 15, 29, 55};
    if (n <= 5)
        return init[n];

    /* small circular buffer that keeps the last seven computed values
       (indices 0 … 6). We only need a(n-6), a(n-4) and a(n-2). */
    seq_t buf[7];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        buf[i % 7] = buf[(i - 2) % 7] + (seq_t)16 * buf[(i - 4) % 7]
                                       - (seq_t)16 * buf[(i - 6) % 7];
    }
    return buf[n % 7];
}
