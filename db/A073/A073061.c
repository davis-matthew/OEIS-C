/*
 * A073061  a(1)=1, a(2k)=2*a(k), a(2k+1)=3*a(k).
 * https://oeis.org/A073061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 2558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073061
   a(1) = 1
   a(2k)   = 2 * a(k)
   a(2k+1) = 3 * a(k)
   This implementation evaluates the recurrence iteratively in O(log n) time
   by processing the bits of n from least significant to most significant. */
seq_t a(idx_t n)
{
    /* Base case: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    /* Process n while it is greater than 1,
       multiplying by 2 for even steps and by 3 for odd steps. */
    while (n > 1)
    {
        if (n & 1ULL)          /* n is odd: a(2k+1) = 3 * a(k) */
            res = (seq_t)(res * (seq_t)3);
        else                   /* n is even: a(2k) = 2 * a(k) */
            res = (seq_t)(res * (seq_t)2);

        n >>= 1;               /* k = floor(n/2) for the next iteration */
    }
    return res;
}
