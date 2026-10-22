/*
 * A002944  a(n) = LCM(1,2,...,n) / n.
 * https://oeis.org/A002944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003418(n)/n = Product_{p prime <= n} p^(floor(log_p(n))
 *            - v_p(n))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   is_prime_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_simple.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t p = 2; p <= n; p++) {
        if (!is_prime_simple(p)) continue;
        /* e = largest k with p^k <= n */
        idx_t e = 1;
        idx_t pk = p;
        while (pk <= n / p) {
            pk *= p;
            e++;
        }
        /* v = exponent of p in n */
        idx_t v = 0;
        idx_t m = n;
        while (m % p == 0) {
            m /= p;
            v++;
        }
        for (idx_t k = v; k < e; k++) {
            result *= (seq_t)p;
        }
    }
    return result;
}
