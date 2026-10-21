/*
 * A200866  Number of 0..3 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors or less than both
 *          neighbors.
 * https://oeis.org/A200866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-4) + a(n-5) for n > 5 Initial
 *            values: a(1)=36, a(2)=94, a(3)=236, a(4)=602, a(5)=1528 The
 *            recurrence uses only the five previous terms; we keep them in a
 *            circular buffer of size six so no dynamic allocation is needed
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; values for n < 1 are undefined, return 0 */
    if (n < 1) return 0;

    /* Return the five known initial values directly */
    if (n == 1) return 36;
    if (n == 2) return 94;
    if (n == 3) return 236;
    if (n == 4) return 602;
    if (n == 5) return 1528;

    /* Circular buffer for the last six terms (indices 0..5) */
    seq_t buf[6];

    /* Fill the buffer with the known initial terms:
       buf[1] = a(1), buf[2] = a(2), ..., buf[5] = a(5)   */
    buf[1] = 36;
    buf[2] = 94;
    buf[3] = 236;
    buf[4] = 602;
    buf[5] = 1528;

    /* Compute the terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        idx_t idx   = i % 6;        /* position to store a(i) */
        seq_t a1 = buf[(i - 1) % 6];
        seq_t a2 = buf[(i - 2) % 6];
        seq_t a4 = buf[(i - 4) % 6];
        seq_t a5 = buf[(i - 5) % 6];
        buf[idx] = 2 * a1 + a2 + 2 * a4 + a5;
    }

    return buf[n % 6];
}
