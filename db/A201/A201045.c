/*
 * A201045  Number of -n..n arrays of 6 elements with adjacent element
 *          differences also in -n..n.
 * https://oeis.org/A201045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n>7, with initial values a(1)=239,
 *            a(2)=4569, a(3)=33305, a(4)=148433, a(5)=491429, a(6)=1333683,
 *            a(7)=3139529
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        239, 4569, 33305, 148433, 491429, 1333683, 3139529
    };

    if (n <= 7)
        return init[n - 1];

    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        /* separate positive and negative contributions to avoid
           unsigned underflow when subtracting. */
        seq_t pos = (seq_t)7 * buf[6] + (seq_t)35 * buf[4] + (seq_t)21 * buf[2] + buf[0];
        seq_t neg = (seq_t)21 * buf[5] + (seq_t)35 * buf[3] + (seq_t)7  * buf[1];
        seq_t next = pos - neg;

        /* shift buffer left, discarding the oldest term. */
        for (idx_t j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = next;
    }

    return buf[6];
}
