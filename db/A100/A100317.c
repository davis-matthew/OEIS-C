/*
 * A100317  Numbers k such that exactly one of k - 1 and k + 1 is prime.
 * https://oeis.org/A100317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that exactly one of k-1
 *            and k+1 is prime
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as safe fallback */

    idx_t cnt = 0;
    seq_t k = 1;                    /* start from 1 (a(1) = 1) */

    while (1) {
        bool prime_minus = false;
        bool prime_plus  = false;

        /* Check if k-1 is prime (k>1) */
        if (k > 1) {
            prime_minus = is_prime_uint64((uint64_t)(k - 1));
        }

        /* Check if k+1 is prime */
        prime_plus = is_prime_uint64((uint64_t)(k + 1));

        /* Exactly one of them prime? */
        if (prime_minus ^ prime_plus) {
            cnt++;
            if (cnt == n) {
                return k;       /* return the n-th such number */
            }
        }
        k++;
    }
}
