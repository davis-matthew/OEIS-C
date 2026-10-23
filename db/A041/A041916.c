/*
 * A041916  Numerators of continued fraction convergents to sqrt(480).
 * https://oeis.org/A041916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac, less
 * Formula:   a(n) = 482*a(n-4)-a(n-8) a(0)=21, a(1)=22, a(2)=219, a(3)=241,
 *            a(4)=10341, a(5)=10582, a(6)=105579, a(7)=116161
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 0..7 */
    static const seq_t init[8] = {
        (seq_t)21,   (seq_t)22,   (seq_t)219,   (seq_t)241,
        (seq_t)10341, (seq_t)10582, (seq_t)105579, (seq_t)116161
    };

    /* Return immediately for the first 8 terms */
    if (n < 8) return init[n];

    /* Circular buffer of the last 8 computed values */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = init[i];

    /* Compute iteratively using the recurrence */
    for (idx_t i = 8; i <= n; ++i)
    {
        /* (i-4)&7 and (i-8)&7 give the correct circular indices */
        seq_t val = (seq_t)482 * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = val;
    }

    /* The desired term is now in buf[n & 7] */
    return buf[n & 7];
}
