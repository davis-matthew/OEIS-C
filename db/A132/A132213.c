/*
 * A132213  Number of distinct primes among the squares mod n.
 * https://oeis.org/A132213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, look
 * Formula:   a(n) = the number of distinct primes that appear as k^2 mod n
 *            for 0 ≤ k < n
 * Range:     u8 n <= 3498, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    size_t sz = (size_t)n;
    uint8_t *seen = (uint8_t *)malloc(sz);
    if (!seen)
        return 0;  /* Out‑of‑memory fallback */

    for (size_t i = 0; i < sz; ++i)
        seen[i] = 0;

    seq_t distinct = 0;

    for (idx_t k = 0; k < n; ++k) {
        idx_t r = mod_mul(k, k, n);   /* (k*k) mod n */
        if (r >= 2 && is_prime(r)) {
            if (!seen[(size_t)r]) {
                seen[(size_t)r] = 1;
                ++distinct;
            }
        }
    }

    free(seen);
    return distinct;
}
