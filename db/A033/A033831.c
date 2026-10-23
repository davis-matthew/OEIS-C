/*
 * A033831  Number of numbers d dividing n such that d >= 3 and n/d <= d-2.
 * https://oeis.org/A033831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors d of n such that d >= 3 and n/d <= d-2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t i;

    /* iterate over divisors d1 <= d2 */
    for (i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;          /* smaller divisor */
            idx_t d2 = n / i;      /* larger divisor, may equal d1 */

            /* test divisor d1 */
            if (d1 >= 3)
            {
                /* n/d1 == d2, check condition n/d1 <= d1-2
                   i.e., d2 <= d1-2 */
                if (d2 <= d1 - 2)
                    ++count;
            }

            /* test divisor d2 if distinct */
            if (d2 != d1 && d2 >= 3)
            {
                /* n/d2 == d1, check condition d1 <= d2-2 */
                if (d1 <= d2 - 2)
                    ++count;
            }
        }
    }

    return count;
}
