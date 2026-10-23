/*
 * A024930  a(n) = sum of remainders of n mod 1,3,5,...,2k-1, where k = [
 *          (n+1)/2 ].
 * https://oeis.org/A024930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=0}^{k-1} (n % (2*j+1)), where k = floor((n+1)/2)
 * Range:     u8 n <= 55, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* outside the sequence offset */

    seq_t sum   = 0;
    idx_t k     = (n + 1) / 2;          /* number of odd divisors to use  */
    seq_t n_seq = (seq_t)n;              /* convert index to seq_t for modulo */

    for (idx_t j = 0; j < k; ++j)
    {
        idx_t d = 2 * j + 1;            /* odd divisor 1,3,5,... */
        sum += n_seq % (seq_t)d;
    }

    return sum;
}
