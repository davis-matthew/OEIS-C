/*
 * A081307  a(n) = (n+1)*tau(n) - sigma(n).
 * https://oeis.org/A081307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*tau(n) - sigma(n) where tau(n) = number of divisors
 *            of n sigma(n)= sum of all divisors of n
 * Range:     u8 n <= 47, u16 n <= 2015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count divisors and sum of divisors by iterating up to sqrt(n).  This
   runs in O(sqrt(n)) time, which is comfortably fast for n ≤ 10 000.  */

seq_t a(idx_t n)
{
    /* Convert n to a native type for arithmetic */
    uint64_t nn = (uint64_t)n;
    uint64_t tau   = 0;
    uint64_t sigma = 0;

    /* For each i up to sqrt(n) check if it divides n */
    for (uint64_t i = 1; i * i <= nn; ++i)
    {
        if (nn % i == 0)
        {
            uint64_t j = nn / i;
            /* Divisor count */
            tau += (i == j ? 1 : 2);

            /* Sum of divisors */
            sigma += i;
            if (j != i)
                sigma += j;
        }
    }

    /* Compute the final value using seq_t arithmetic only */
    return ((seq_t)(n + 1)) * (seq_t)tau - (seq_t)sigma;
}
