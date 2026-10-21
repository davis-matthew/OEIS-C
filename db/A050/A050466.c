/*
 * A050466  a(n) = Sum_{d|n, n/d=3 mod 4} d^3.
 * https://oeis.org/A050466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, (n/d mod 4) = 3} d³
 * Range:     u8 n <= 20, u16 n <= 122, u32 n <= 4766, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the value of sequence A050466 for the given index n.
   The function is O(sqrt(n)) and works for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of sequence bounds – return 0 */

    seq_t sum = 0;                    /* accumulator for the sum */

    /* iterate over divisors up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;          /* the complementary divisor */

            /* Check the condition for d (when d is the divisor) */
            if ((q & 3ULL) == 3) {
                seq_t dd = (seq_t)d;
                sum += dd * dd * dd;
            }

            /* If d and q are different, also check the complementary divisor */
            if (d != q) {
                /* now q is the divisor, d is the quotient   */
                if ((d & 3ULL) == 3) {
                    seq_t qq = (seq_t)q;
                    sum += qq * qq * qq;
                }
            }
        }
    }
    return sum;
}
