/*
 * A041681  Denominators of continued fraction convergents to sqrt(359).
 * https://oeis.org/A041681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 720*a(n-4) - a(n-8) for n>7, a(0) = 1, a(1) = 1, a(2) =
 *            18, a(3) = 19, a(4) = 702, a(5) = 721, a(6) = 12959, a(7) =
 *            13680
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial eight terms */
    static const seq_t init[8] = {
        (seq_t)1,  (seq_t)1,  (seq_t)18,  (seq_t)19,
        (seq_t)702, (seq_t)721, (seq_t)12959, (seq_t)13680
    };

    if (n < 8)
        return init[n];

    /* Circular buffer of the last eight values: buf[i] is a[i] */
    seq_t buf[8];
    for (int k = 0; k < 8; ++k)
        buf[k] = init[k];

    seq_t val;
    for (idx_t i = 8; i <= n; ++i) {
        val = ((seq_t)720) * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = val;
    }
    return buf[n & 7];
}
