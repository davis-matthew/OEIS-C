/*
 * A071263  Smallest nontrivial composite number beginning with n.
 * https://oeis.org/A071263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest composite integer whose decimal representation
 *            begins with the decimal representation of n, excluding n itself
 *            and the single‐digit suffix n0 (i.e., n followed by one 0). This
 *            is computed by exhaustive search over increasing suffix lengths,
 *            testing each candidate for compositeness
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* pow10 = 10^len of the suffix */
    seq_t pow10 = 10;          /* len = 1 first */
    seq_t start, x;
    while (1) {
        start = (seq_t)n * pow10;   /* n followed by len zeros */

        /* iterate over suffix digits from 0 to pow10-1
           but skip the forbidden case n0 (len==1 && suffix==0) */
        for (seq_t suffix = (pow10 == 10) ? 1 : 0; suffix < pow10; ++suffix) {
            x = start + suffix;
            if (x <= 1) continue;                /* ignore 0,1 */
            if (!isPrimeGeneric(x)) return x;     /* composite found */
        }

        /* increase suffix length: multiply by 10 */
        pow10 *= 10;
    }
}
