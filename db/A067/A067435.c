/*
 * A067435  a(n) is the sum of all the remainders when n-th odd number is
 *          divided by odd numbers < 2n-1.
 * https://oeis.org/A067435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{odd d, 1 ≤ d < 2n−1} (2n−1) mod d
 * Range:     u8 n <= 28, u16 n <= 431, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t m = (seq_t)n * 2 - 1;          /* 2n−1, the nth odd number */
    /* iterate over odd divisors d = 1,3,5,... less than m */
    for (idx_t d = 1; (seq_t)d < m; d += 2)
    {
        sum += m % (seq_t)d;
    }
    return sum;
}
