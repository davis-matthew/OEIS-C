/*
 * A089967  Numbers k such that Omega(k) = gpf(k) mod lpf(n), where
 *          Omega=A001222, gpf=A006530 and lpf=A020639.
 * https://oeis.org/A089967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that Omega(k) = gpf(k)
 *            mod lpf(k)
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   bigomega, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/largest_prime_factor.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but return 0 for safety */
    idx_t count = 0;
    uint64_t k = 1;
    while (1) {
        seq_t omega = bigomega(k);
        uint64_t g = largest_prime_factor(k);
        uint64_t l = lpf(k);
        /* avoid division by zero if l==0; treat as mismatch */
        if (l != 0 && (g % l) == (uint64_t)omega) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
}
