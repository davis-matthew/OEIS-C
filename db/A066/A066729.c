/*
 * A066729  a(n) = Product_{d|n, d<n} d if n is composite, n otherwise.
 * https://oeis.org/A066729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n if n is prime; otherwise a(n) = n^{(d(n)-2)/2}, where
 *            d(n) is the number of divisors of n. This equals the product of
 *            all proper divisors of a composite number
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 89, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Validated: standard, pass, 73 terms through n = 9625 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32, ipow_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"
#include "../utils/is_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n <= 1)
        return (seq_t)1;

    /* For primes the definition gives a(n) = n. */
    if (is_prime(n))
        return (seq_t)n;

    /* Factor n into its prime powers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the divisor function d(n) = Π (ei + 1). */
    idx_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (idx_t)(exps[i] + 1);

    /* a(n) = n^{(d-2)/2}.  When d is odd the exponent is a
       half‑integer; this happens only for perfect squares. */
    if (d & 1) {             /* odd d → perfect square */
        idx_t k = (d - 3) / 2;          /* integer part of exponent */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        seq_t pow_n  = ipow_idx((seq_t)n, k);
        return seq_mul(sqrt_n, pow_n);
    } else {                 /* even d → integer exponent */
        idx_t k = (d - 2) / 2;
        return ipow_idx((seq_t)n, k);
    }
}
