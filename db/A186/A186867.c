/*
 * A186867  Number of 8-step king's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 550504*n^2 - 3839372*n + 6382124 (for n > 6); otherwise
 *            hard‑coded values from the sequence
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the few small n with exact values */
    if (n <= 6) {
        if (n == 1u) return (seq_t)0;
        if (n == 2u) return (seq_t)0;
        if (n == 3u) return (seq_t)2384;
        if (n == 4u) return (seq_t)183472;
        if (n == 5u) return (seq_t)1110000;
        if (n == 6u) return (seq_t)3193800;
        /* unreachable, but keeps the compiler happy */
        return (seq_t)0;
    }

    /* For n > 6 use the quadratic formula */
    seq_t sn = (seq_t)n;
    const seq_t c1 = 550504ULL;
    const seq_t c2 = 3839372ULL;
    const seq_t c3 = 6382124ULL;

    return c1 * sn * sn - c2 * sn + c3;
}
