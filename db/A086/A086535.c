/*
 * A086535  Product of numbers obtained by adding one to the odd divisors of n
 *          and subtracting 1 from the even divisors.
 * https://oeis.org/A086535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = ∏_{d|n} (d-1 if d even, d+1 if d odd)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function iterates over all divisors of n, applies the
   appropriate transformation, and returns their product. */
seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* process divisor d1 */
            seq_t t1 = (d1 & 1) ? ((seq_t)d1 + 1) : ((seq_t)d1 - 1);
            prod *= t1;

            /* if d1 != d2, process divisor d2 */
            if (d1 != d2) {
                seq_t t2 = (d2 & 1) ? ((seq_t)d2 + 1) : ((seq_t)d2 - 1);
                prod *= t2;
            }
        }
    }
    return prod;
}
