/*
 * A090086  Smallest pseudoprime to base n, not necessarily exceeding n (cf.
 *          A007535).
 * https://oeis.org/A090086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LeastComposite{x; n^(x-1) mod x = 1}
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9906 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the smallest composite x such that n^(x-1) ≡ 1 (mod x).  */
/*  The search proceeds in increasing order starting at 4.       */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <stddef.h>
#include <limits.h>
#include <string.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/powmod.h"

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

/* External linkage as required.  */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* base n (as seq_t) */
    for (uint64_t xi = 4; ; ++xi) /* candidate composite x, 64‑bit counter */
    {
        seq_t x = (seq_t)xi;           /* current candidate as seq_t */

        /* skip primes and numbers < 4 */
        if (x < 4 || isPrimeGeneric(x))
            continue;

        /* test the congruence */
        seq_t r = powmod(nn, xi - 1, x);   /* n^(x-1) mod x */
        if (r == 1)
            return x;                       /* found the smallest composite */
    }
}
