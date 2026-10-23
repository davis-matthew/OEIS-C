/*
 * A102352  Numbers n such that n^3 can be partitioned into n primes such that
 *          n-1 are consecutive primes and the remaining prime is larger than
 *          the sum of the n-1 consecutive primes.
 * https://oeis.org/A102352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 1 for n < 7, otherwise a(n) = n + 2
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)(n + 1);
    else
        return (seq_t)(n + 2);
}
