/*
 * A168066  If n = Product p(k)^e(k) then a(n) = (Product (p(k)+1)^e(k) -
 *          Product (p(k)-1)^e(k))/2, a(1) = 0.
 * https://oeis.org/A168066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏(p+1)^e - ∏(p-1)^e)/2 for n = ∏p^e, a(1)=0
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 9850, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 1) return 0;               /* a(1) = 0 by definition */

    uint64_t m = n;                     /* mutable copy of the index */
    seq_t prod_plus  = 1;
    seq_t prod_minus = 1;

    /* factor m by trial division (adequate for m up to about 10 000) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t base_plus  = (seq_t)(p + 1);
            seq_t base_minus = (seq_t)(p - 1);

            seq_t pow_plus  = 1;
            seq_t pow_minus = 1;
            for (uint64_t i = 0; i < e; ++i) {
                pow_plus  *= base_plus;
                pow_minus *= base_minus;
            }

            prod_plus  *= pow_plus;
            prod_minus *= pow_minus;
        }
    }

    /* any remaining factor is prime */
    if (m > 1) {
        seq_t base_plus  = (seq_t)(m + 1);
        seq_t base_minus = (seq_t)(m - 1);
        prod_plus  *= base_plus;
        prod_minus *= base_minus;
    }

    return (prod_plus - prod_minus) / 2;
}
