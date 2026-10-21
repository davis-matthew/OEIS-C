/*
 * A056899  Primes of the form k^2 + 2.
 * https://oeis.org/A056899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k² + 2, where k is the n‑th integer such that k²+2 is
 *            prime
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 24, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0; return 0 for safety */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        /* compute k² + 2 using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 tmp = (unsigned __int128)k * k + 2;
        seq_t val = (seq_t)(uint64_t)tmp;   /* cast down – valid for n up to ~10000 */

        if (is_prime_big(val)) {            /* test primality */
            ++count;
            if (count == n)
                return val;
        }
        ++k;
    }
}
