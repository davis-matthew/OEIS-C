/*
 * A122255  Characteristic function of numbers with 3-smooth Euler's totient
 *          (A000010).
 * https://oeis.org/A122255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if Euler's totient φ(n) is a 3‑smooth number, that is
 *            φ(n) = 2^a·3^b for some a,b ≥ 0; otherwise a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_3_smooth_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation does not rely on any external utilities. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_3_smooth_uint64.h"

static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t temp = n;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        result -= result >> 1;          /* result = n - n/2 */
        while ((temp & 1ULL) == 0)
            temp >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* remaining prime larger than sqrt(n) */
    if (temp > 1)
        result -= result / temp;

    return result;
}

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1 of the OEIS sequence) */
    uint64_t phi_val = phi_uint64((uint64_t)n);
    return (seq_t)(is_3_smooth_uint64(phi_val) ? 1 : 0);
}
