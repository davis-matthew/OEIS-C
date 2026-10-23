/*
 * A042303  Denominators of continued fraction convergents to sqrt(678).
 * https://oeis.org/A042303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1354*a(n-2) - a(n-4) for n > 3, with a(0) = 1, a(1) = 26,
 *            a(2) = 1353, a(3) = 35204
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values (constant). */
    static const seq_t init[4] = { (seq_t)1, (seq_t)26, (seq_t)1353, (seq_t)35204 };

    if (n < 4)
        return init[n];

    /* Circular buffer holding the last four computed terms. */
    seq_t buf[4];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];

    for (idx_t i = 4; i <= n; ++i) {
        /* Indices modulo 4: i-2 -> (i-2)&3, i-4 -> i&3 */
        seq_t a1 = buf[(i - 2) & 3];
        seq_t a2 = buf[i & 3];
        seq_t next = (seq_t)1354 * a1 - a2;
        buf[i & 3] = next;
    }

    return buf[n & 3];
}
