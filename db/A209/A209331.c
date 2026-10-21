/*
 * A209331  a(n) = Sum_{k=0..[n/2]} binomial((n-k)^2, n*k-k^2).
 * https://oeis.org/A209331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial((n-k)^2 , n*k−k^2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the external binomial(idx_t , idx_t) routine provided in ../utils/binomial.h */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - k;                     /* m = n - k */
        idx_t top = m * m;                   /* top = (n - k)^2 */
        idx_t bottom = n * k - k * k;        /* bottom = n*k - k^2 */
        sum += binomial(top, bottom);
    }
    return sum;
}
