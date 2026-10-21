/*
 * A234959  Highest power of 6 dividing n.
 * https://oeis.org/A234959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(valuation(n,6))
 * Range:     u8 n <= 1295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count exponent of 2 in n */
    idx_t v2 = 0;
    idx_t t = n;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        ++v2;
    }

    /* Count exponent of 3 in n */
    idx_t v3 = 0;
    t = n;
    while (t % 3ULL == 0) {
        t /= 3;
        ++v3;
    }

    /* Exponent of 6 is the minimum of the two */
    idx_t k = (v2 < v3) ? v2 : v3;

    /* Compute 6^k */
    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i) {
        result *= (seq_t)6;
    }
    return result;
}
