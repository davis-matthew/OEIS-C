/*
 * A042728  Numerators of continued fraction convergents to sqrt(894).
 * https://oeis.org/A042728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 598*a(n-4) - a(n-8) Initial values: a(0)=29, a(1)=30,
 *            a(2)=269, a(3)=299, a(4)=17611, a(5)=17910, a(6)=160891,
 *            a(7)=178801
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constant initial block of 8 terms */
    static const seq_t init[8] = {
        (seq_t)29,   (seq_t)30,   (seq_t)269,   (seq_t)299,
        (seq_t)17611,(seq_t)17910,(seq_t)160891,(seq_t)178801
    };

    /* Direct lookup for the first 8 indices */
    if (n < 8U)
        return init[n];

    /* Circular buffer of the last 8 computed terms */
    seq_t buf[8];
    for (idx_t i = 0; i < 8U; ++i)
        buf[i] = init[i];

    /* Compute terms from a(8) up to a(n) using the recurrence */
    for (idx_t i = 8U; i <= n; ++i) {
        seq_t term = (seq_t)598 * buf[(i - 4U) & 7U] - buf[(i - 8U) & 7U];
        buf[i & 7U] = term;
    }

    /* The desired term is now in the buffer position corresponding to n */
    return buf[n & 7U];
}
