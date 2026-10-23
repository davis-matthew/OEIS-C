/*
 * A173392  Product of nonzero remainders of n mod k, for k = 1,2,3,...,n.
 * https://oeis.org/A173392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n, n%k ≠ 0} (n % k)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    seq_t sn   = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t r = sn % (seq_t)k;
        if (r != (seq_t)0) {
            prod *= r;
        }
    }
    return prod;
}
