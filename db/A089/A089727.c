/*
 * A089727  Largest prime of the form n*k+1, k <= n.
 * https://oeis.org/A089727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   largest prime of the form n*k+1 with 1 ≤ k ≤ n
 * Range:     u8 n <= 15, u16 n <= 248, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but avoid UB */
    for (idx_t k = n; k > 0; --k) {
        seq_t val = ((seq_t)n * (seq_t)k) + 1;
        if (is_prime_big(val))
            return val;
    }
    return 0;   /* should never happen for n ≥ 1 */
}
