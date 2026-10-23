/*
 * A179132  Denominators of A178381(4*n+1)/A178381(4*n).
 * https://oeis.org/A179132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 18*a(n-3) - a(n-6) for n > 6 Initial values: a(0)=1,
 *            a(1)=3, a(2)=14, a(3)=36, a(4)=47, a(5)=246, a(6)=644
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms for n = 0 … 6 */
    const seq_t init[7] = { (seq_t)1, (seq_t)3, (seq_t)14,
                            (seq_t)36, (seq_t)47, (seq_t)246,
                            (seq_t)644 };

    /* Direct lookup for the first seven terms */
    if (n <= 6)
        return init[(int)n];

    /* Circular buffer of the last seven terms.
       buf[i] holds a(i) for the current i modulo 7. */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = init[i];

    /* Iterate from 7 up to n */
    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) = 18*a(i-3) - a(i-6) */
        seq_t next = (seq_t)18 * buf[(int)(i - 3) % 7] - buf[(int)(i - 6) % 7];
        buf[(int)(i % 7)] = next;
    }

    /* Return a(n) */
    return buf[(int)(n % 7)];
}
