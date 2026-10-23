/*
 * A115206  Least number d such that prime(n) -/+ 2d form a prime pair;
 *          prime(n) being the n-th prime.
 * https://oeis.org/A115206
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal d such that prime(n) - 2d and prime(n) + 2d are
 *            both prime
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;

    idx_t p = (idx_t)get_prime(n);   /* nth prime */
    for (idx_t d = 1; ; ++d) {
        idx_t lower = p - 2*d;
        idx_t upper = p + 2*d;
        if (lower >= 2 && is_prime(lower) && is_prime(upper))
            return (seq_t)d;
    }
}
