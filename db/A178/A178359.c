/*
 * A178359  Rounded up arithmetic mean of digits of n appended to n, cf.
 *          A004427.
 * https://oeis.org/A178359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10*n + ceil( sum_of_digits(n) / num_digits(n) )
 * Range:     u8 n <= 25, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which is equivalent to a(n) = A008592(n) + A004427(n) */

seq_t a(idx_t n)
{
    /* Compute sum of digits and digit count */
    idx_t t = n;
    unsigned sum = 0;
    unsigned count = 0;

    if (t == 0)                /* special case: 0 has one digit 0 */
    {
        sum   = 0;
        count = 1;
    }
    else
    {
        while (t != 0)
        {
            sum   += (unsigned)(t % 10);
            count += 1;
            t     /= 10;
        }
    }

    /* Ceil of the average digit */
    unsigned digitMean = (sum + count - 1) / count;

    /* Append the digitMean to n: a = 10*n + digitMean */
    return (seq_t)n * 10 + (seq_t)digitMean;
}
