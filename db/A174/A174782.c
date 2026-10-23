/*
 * A174782  Sum of the numerators for computing the fourth moment of the
 *          probability mass function for the number of involutions with k
 *          2-cycles in n elements (A000085) assuming equal likelihood.
 * https://oeis.org/A174782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^4 * n! / ((n-2*k)! * 2^k * k!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    seq_t T = 1;                      /* T(k) = n! / ((n-2k)! * 2^k * k!) */
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t mult = (seq_t)(n - 2*k + 2) * (seq_t)(n - 2*k + 1);
        T *= mult;                     /* multiply by (n-2k+2)(n-2k+1)         */
        T /= (seq_t)(2 * k);           /* divide by 2k (exact division)         */
        seq_t k4 = (seq_t)k * k * k * k;
        sum += T * k4;                 /* add k^4 * T(k) to the sum             */
    }
    return sum;
}
