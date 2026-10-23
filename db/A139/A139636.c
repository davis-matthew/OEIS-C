/*
 * A139636  If n = the k-th prime, then a(n) = the (k+1)th prime. If n = the
 *          k-th composite, then a(n) = the (k+1)th composite.
 * https://oeis.org/A139636
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   if n is prime, a(n) = next prime; if n is composite, a(n) = next
 *            composite
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n<2, acceptable per spec */

    bool prime = is_prime(n);
    if (prime) {
        for (idx_t m = n + 1;; ++m)
            if (is_prime(m))
                return (seq_t)m;
    } else {
        for (idx_t m = n + 1;; ++m)
            if (!is_prime(m))
                return (seq_t)m;
    }
}
