/*
 * A225767  Least k>0 such that k^5+n is prime, or 0 if k^5+n is never prime.
 * https://oeis.org/A225767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that k^5 + n is prime, or 0 if no such k
 *            exists. The only values for which no such k exists are n = 0 or
 *            n being a perfect fifth power
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, is_perfect_fifth, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow_uint.h"
#include "../utils/is_perfect_fifth.h"

seq_t a(idx_t n)
{
    /* n = 0 has no k producing a prime (k^5 is never prime) */
    if (n == 0) return 0;

    /* if n is a perfect fifth power, x^5 + n is reducible and never prime */
    if (is_perfect_fifth(n)) return 0;

    for (idx_t k = 1;; ++k) {
        seq_t val;

        /* compute k^5 carefully, avoiding overflow for 64‑bit builds */
        if (sizeof(seq_t) == sizeof(uint64_t)) {
            __int128 power = 1;
            for (int i = 0; i < 5; ++i) power *= k;
            if (power > UINT64_MAX) return 0;   /* exceed representable range */
            __int128 sum = power + n;
            if (sum > UINT64_MAX) return 0;     /* exceed representable range */
            val = (seq_t)sum;
        } else {
            /* generic big‑integer arithmetic for arbitrary‑precision seq_t */
            val = pow_uint(k, 5);
            val += (seq_t)n;
        }

        if (isPrimeGeneric(val))
            return (seq_t)k;
    }
}
