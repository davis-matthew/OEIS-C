/*
 * A102374  Numbers k such that k77777 is prime.
 * https://oeis.org/A102374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Numbers k such that k77777 is prime
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate k = 1,2,..., and count the cases where
   k*100000 + 77777 is prime using the provided is_prime_u64 helper.
   Return the k corresponding to the nth such occurrence. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 specifies a(1) as first term */

    idx_t    count = 0;            /* number of found terms */
    uint64_t k = 1;                /* candidate k */

    /* Upper limit to avoid overflow of k*100000+77777 (within 64‑bit). */
    const uint64_t LIM = UINT64_MAX / 100000ULL;

    for (; k <= LIM; ++k) {
        uint64_t num = k * 100000ULL + 77777ULL;
        if (is_prime_u64(num)) {
            ++count;
            if (count == n) {
                return (seq_t)k;   /* cast fits safely for n up to ~10⁴ */
            }
        }
    }

    /* If we exceed 64‑bit range, we cannot continue safely. Return zero */
    return 0;
}
