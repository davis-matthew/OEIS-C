/*
 * A085562  Sum of the nonprime digits of n.
 * https://oeis.org/A085562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of all decimal digits of n that are not prime digits
 *            (2,3,5,7)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;           /* accumulator in seq_t type */
    idx_t x = n;             /* work on a copy of n */

    while (x > 0)
    {
        int d = x % 10;      /* extract lowest decimal digit (int is ample for 0..9) */

        /* add digit if it is not a prime digit */
        if (d != 2 && d != 3 && d != 5 && d != 7)
            sum += (seq_t)d;

        x /= 10;            /* move to the next digit */
    }

    return sum;
}
