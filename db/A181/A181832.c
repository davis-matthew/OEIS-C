/*
 * A181832  The product of the positive integers <= n that are strongly prime
 *          to n.
 * https://oeis.org/A181832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of integers k (1 <= k <= n) such that gcd(k,n) =
 *            1 and k does NOT divide (n - 1). Equivalently, the product of
 *            the numbers <= n that are strongly prime to n
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 28, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;  /* empty product */

    seq_t prod = (seq_t)1;
    seq_t sn = (seq_t)n;
    idx_t n_minus1 = n - 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* check strongly prime: gcd(k,n) == 1 and k does not divide n-1 */
        if (gcd((seq_t)k, sn) == 1 && (n_minus1 % k) != 0) {
            prod *= (seq_t)k;
        }
    }
    return prod;
}
