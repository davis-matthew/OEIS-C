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
 * Validated: standard, pass, 116 terms through n = 9559 (2026-11-03)
 * Helpers:   ipow_u64_idx, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"
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

/* check whether n is a perfect fifth power (i.e. ∃x : x^5 = n) */
static bool is_perfect_fifth(idx_t n)
{
    if (n <= 1) return false;           /* 0^5 and 1^5 do not yield a prime sum */

    idx_t low = 0, high = 0;
    /* set high to the largest x such that x^5 ≤ n, using an upper bound */
    /* Since x^5 grows fast, x <= 10^4 suffices for n up to 10^20 and all practical cases */
    high = (idx_t)0x10000;              /* 65536 > 10^4 */

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = 1;
        for (int i = 0; i < 5; ++i) val *= (__int128)mid;
        if (val == n) return true;
        if (val < n) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

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
            val = ipow_u64_idx(k, 5);
            val += (seq_t)n;
        }

        if (isPrimeGeneric(val))
            return (seq_t)k;
    }
}
