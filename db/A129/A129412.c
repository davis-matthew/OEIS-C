/*
 * A129412  Numbers k such that mean of 7 consecutive squares starting with
 *          k^2 is prime.
 * https://oeis.org/A129412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k where mean of 7 consecutive squares starting with k² is
 *            prime * That mean equals (k+3)² + 4. Hence a(n) consists of all
 *            k such that (k+3)² + 4 is prime
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 4995, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8518 (2026-11-03)
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
    idx_t k = 0;      /* current candidate k */
    idx_t cnt = 0;    /* number of found terms */

    while (cnt < n) {
        seq_t kk = (seq_t)k;          /* cast k to seq_t for arithmetic */
        seq_t val = (kk + 3) * (kk + 3) + 4;   /* (k+3)² + 4 */

        if (isPrimeGeneric(val)) {    /* test primality */
            cnt++;
            if (cnt == n)
                return kk;           /* kth term found */
        }
        k++;                          /* try next k */
    }

    /* Should never reach here for valid n ≥ 1 */
    return (seq_t)0;
}
