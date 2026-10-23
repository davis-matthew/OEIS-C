/*
 * A163942  Fifth right hand column of triangle A163940.
 * https://oeis.org/A163942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/18)*(3^(n+6) + (3*n-10)*4^(n+3) - 9*2^(n+3) + 1)
 *            Recurrence: a(n) = 14*a(n-1) - 75*a(n-2) + 190*a(n-3) -
 *            224*a(n-4) + 96*a(n-5), with a(0)=1, a(1)=14, a(2)=121,
 *            a(3)=834, a(4)=5037
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial five terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)14, (seq_t)121, (seq_t)834, (seq_t)5037 };

    if (n < 5u)
        return init[n];

    /* Use a circular buffer to keep the last five values */
    seq_t buf[5];
    for (idx_t i = 0; i < 5; ++i)
        buf[i] = init[i];

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t a1 = buf[(i - 1u) % 5u];
        seq_t a2 = buf[(i - 2u) % 5u];
        seq_t a3 = buf[(i - 3u) % 5u];
        seq_t a4 = buf[(i - 4u) % 5u];
        seq_t a5 = buf[(i - 5u) % 5u];

        /* Compute positive and negative parts separately to avoid unsigned underflow */
        seq_t pos = (seq_t)14 * a1 + (seq_t)190 * a3 + (seq_t)96 * a5;
        seq_t neg = (seq_t)75 * a2 + (seq_t)224 * a4;

        buf[i % 5u] = pos - neg;
    }

    return buf[n % 5u];
}
