/*
 * A227913  Sum of all k from {1,...,n} such that the fractional part of n/k
 *          is contained in the open interval (1/4, 3/4).
 * https://oeis.org/A227913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum_{k=1..n} k where fractional part of n/k lies in (1/4,3/4) */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;                 /* remainder of n divided by k */
        seq_t sr = (seq_t)r;
        if (sr * (seq_t)4 > (seq_t)k && sr * (seq_t)4 < (seq_t)3 * (seq_t)k) {
            sum += (seq_t)k;
        }
    }
    return sum;
}
