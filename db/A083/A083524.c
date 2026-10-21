/*
 * A083524  a(1) = 1, a(n) = a(n-1)*n if n is a prime, a(n) = a(n-1)+ n if n
 *          is composite.
 * https://oeis.org/A083524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) * n if n is prime, a(n) = a(n-1) + n if
 *            n is composite
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 100,
 *            big any n
 * Helpers:   is_prime_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_idx.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined in OEIS, but safe for caller */
    seq_t val = 1;                  /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime_idx(i))
            val = val * (seq_t)i;
        else
            val = val + (seq_t)i;
    }
    return val;
}
