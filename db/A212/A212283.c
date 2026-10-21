/*
 * A212283  First a(n) > 1 whose sum of digits is the same in base 2 as in
 *          base n.
 * https://oeis.org/A212283
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   computes the first integer >1 whose sum of digits is the same in
 *            base 2 and in base n. The function is defined for n ≥ 2; for n <
 *            2 it returns 0
 * Range:     u8 n <= 62, u16 n <= 373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t x = 2;                  /* start search from 2 */
    while (1)
    {
        /* Sum of binary digits */
        seq_t sum2 = 0;
        seq_t t = x;
        while (t != 0)
        {
            sum2 += (t & 1);
            t >>= 1;
        }

        /* Sum of digits in base n */
        seq_t sumn = 0;
        t = x;
        while (t != 0)
        {
            sumn += (t % n);
            t /= n;
        }

        if (sum2 == sumn)
            return x;

        ++x;
    }
}
