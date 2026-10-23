/*
 * A168054  Expansion of (1-8x^2-24x^3)/((1-2x)^2*(1+2x+4x^2)).
 * https://oeis.org/A168054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-3) - 16*a(n-4) for n > 3 Initial values:
 *            a(0) = 1 a(1) = 2 a(2) = -4 a(3) = -24
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 25, i64 n <= 56, i128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t base[4] =
    {
        (seq_t)1,         /* a(0) */
        (seq_t)2,         /* a(1) */
        (seq_t)-4,        /* a(2) */
        (seq_t)-24        /* a(3) */
    };

    if (n < 4)
        return base[n];

    /* we only need the last five values to compute the next one */
    seq_t buf[5];

    /* initialise the first four entries */
    for (idx_t i = 0; i < 4; ++i)
        buf[i] = base[i];

    /* compute from a(4) up to a(n) */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t val = buf[(i - 1) % 5] * 2
                  + buf[(i - 3) % 5] * 8
                  - buf[(i - 4) % 5] * 16;
        buf[i % 5] = val;
    }

    return buf[n % 5];
}
