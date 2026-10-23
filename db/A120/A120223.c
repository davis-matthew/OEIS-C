/*
 * A120223  a(n) is the minimal number k>1 such that n+k and n*k+1 are primes.
 * https://oeis.org/A120223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal k > 1 such that n + k and n * k + 1 are
 *            primes
 * Range:     u8 n <= 23, u16 n <= 1699, u32 n <= 2531, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* undefined for n = 0, return 0 */
    idx_t k = 2;
    for (;;) {
        seq_t ns = (seq_t)n;
        seq_t nk = ns + (seq_t)k;
        if (!is_prime_big(nk)) {
            ++k;
            continue;
        }
        seq_t nkp1 = ns * (seq_t)k + 1;
        if (is_prime_big(nkp1)) {
            return (seq_t)k;
        }
        ++k;
    }
}
