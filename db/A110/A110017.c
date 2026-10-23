/*
 * A110017  Number of games that can be played with a set of 4n cards.
 * https://oeis.org/A110017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = ((4n)!/(4!)^n + (2n)!/(2!)^n)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod2 = 1;
    seq_t prod4 = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* term for (2n)!/(2!)^n : multiply by (2i-1)(2i)/2 */
        seq_t t = (seq_t)(2 * i - 1);
        prod2 *= t;
        t = (seq_t)(2 * i);
        prod2 *= t;
        prod2 /= 2;

        /* term for (4n)!/(4!)^n : multiply by (4i-3)(4i-2)(4i-1)(4i)/24 */
        t = (seq_t)(4 * i - 3);
        prod4 *= t;
        t = (seq_t)(4 * i - 2);
        prod4 *= t;
        t = (seq_t)(4 * i - 1);
        prod4 *= t;
        t = (seq_t)(4 * i);
        prod4 *= t;
        prod4 /= 24;
    }

    /* final result */
    seq_t sum = prod2 + prod4;
    return sum / 2;
}
