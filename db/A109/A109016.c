/*
 * A109016  Concatenate n and the sum of factorials of the digits of n.
 * https://oeis.org/A109016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate(n, sum of factorials of the digits of n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t fact[10] = {1,1,2,6,24,120,720,5040,40320,362880};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;
    if (temp == 0)
        sum = fact[0];
    else
    {
        while (temp > 0)
        {
            sum += fact[temp % 10];
            temp /= 10;
        }
    }

    /* count decimal digits of sum */
    idx_t k = 0;
    seq_t t = sum;
    while (t > 0)
    {
        k++;
        t /= 10;
    }

    /* compute 10^k */
    seq_t pow10k = 1;
    while (k > 0)
    {
        pow10k *= 10;
        k--;
    }

    return (seq_t)n * pow10k + sum;
}
