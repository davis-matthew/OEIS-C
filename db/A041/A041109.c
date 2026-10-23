/*
 * A041109  Denominators of continued fraction convergents to sqrt(62).
 * https://oeis.org/A041109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   a(n) = 126*a(n-4) - a(n-8); Initial values: a(0)=1, a(1)=1,
 *            a(2)=7, a(3)=8, a(4)=119, a(5)=127, a(6)=881, a(7)=1008
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms for n < 8 */
    static const seq_t base[8] = {
        (seq_t)1,   (seq_t)1,   (seq_t)7,   (seq_t)8,
        (seq_t)119, (seq_t)127, (seq_t)881, (seq_t)1008
    };

    if (n < 8)
        return base[n];

    /* Use a ring buffer of length 8 to compute terms iteratively */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    for (idx_t i = 8; i <= n; ++i) {
        /* Compute a(i) = 126*a(i-4) - a(i-8) */
        seq_t next = (seq_t)126 * buf[(i - 4) % 8] - buf[(i - 8) % 8];
        buf[i % 8] = next;
    }

    return buf[n % 8];
}
