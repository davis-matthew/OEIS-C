/*
 * A219751  Expansion of x^4*(2-3*x-x^2)/((1+x)*(1-2*x)^2).
 * https://oeis.org/A219751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=0, a(4)=2, a(5)=3, a(6)=8; For n≥7: a(n) =
 *            3*a(n-1) - 4*a(n-3)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..6 */
    static const seq_t base[7] = {0, 0, 0, 0, 2, 3, 8};

    if (n < 7)
        return base[n];

    /* Circular buffer holding the last 7 values of a(k). */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = base[i];

    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute a(i) = 3*a(i-1) - 4*a(i-3) */
        seq_t term1 = buf[(i - 1) % 7] * (seq_t)3;
        seq_t term2 = buf[(i - 3) % 7] * (seq_t)4;
        buf[i % 7] = term1 - term2;
    }

    return buf[n % 7];
}
