/*
 * A022331  Index of 2^n within sequence of numbers of form 2^i*3^j (A003586).
 * https://oeis.org/A022331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integers of the form 2^i * 3^j that are <= 2^n
 *            Equivalently: a(n) = sum_{k=0}^{n} (floor(k / log2(3)) + 1)
 * Range:     u8 n <= 27, u16 n <= 454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const double LOG2_3 = 1.584962500721156;   /* log2(3) */
    static const double EPS    = 1e-12;              /* safety margin for rounding */

    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        double diff = (double)(n - i);
        /* floor((n-i)/LOG2_3) with little epsilon to avoid rounding
           downwards when the actual value is very close to an integer. */
        uint64_t jmax = (uint64_t)((diff / LOG2_3) + EPS);
        total += (seq_t)(jmax + 1);
    }
    return total;
}
