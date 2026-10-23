/*
 * A179892  Numbers which are not the sum of three distinct members of twin
 *          primes.
 * https://oeis.org/A179892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n ≤ 14 * a(15) = 16 * a(16) = 17 * a(n) = 2*n - 16
 *            for n ≥ 17 * (All odd numbers ≥19 are sums of three distinct
 *            twin primes, so only * the even numbers and the odd numbers
 *            1,3,5,7,9,11,13,17 remain in the * sequence.)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 14)
        return (seq_t)n;
    if (n == 15)
        return (seq_t)16;
    if (n == 16)
        return (seq_t)17;

    seq_t t = (seq_t)n;
    t = t * 2;
    t = t - 16;
    return t;
}
