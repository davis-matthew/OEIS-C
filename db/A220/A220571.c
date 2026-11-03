/*
 * A220571  Composite numbers that are Brazilian.
 * https://oeis.org/A220571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9753 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220571: Composite numbers that are Brazilian. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* Check if a positive integer n is represented as a repdigit in some base b
   (2 <= b <= n-2) and digit d (1 <= d < b).  This defines a “Brazilian” number. */
static bool is_brazilian(seq_t n)
{
    /* All even numbers ≥ 8 are Brazilian (repdigit "22" with base (n‑2)/2). */
    if (n % 2 == 0 && n >= 8)
        return true;

    /* Only odd n ≥ 9 are left.  First try the simple two‑digit case (k = 2).  */
    /* n = d * (b + 1)  →  b = n/d – 1.  */
    for (seq_t d = 1; d <= n; ++d) {
        if (n % d != 0)
            continue;
        seq_t b = n / d - 1;
        if (b < 2 || b > n - 2)
            continue;
        if (d >= b)
            continue;
        /*  d must be < b, and (n = d*(b+1)) already guarantees k = 2. */
        if (d * (b + 1) == n)
            return true;
    }

    /* Larger lengths k ≥ 3 are a few possibilities only.  */
    /* For each k, b grows only up to roughly n^(1/k).  */
    for (idx_t k = 3; /* no statement */ ; ++k) {
        /* If 2^k – 1 > n, further k will not produce any value ≤ n.  */
        seq_t pow2 = 1;
        for (idx_t i = 0; i < k; ++i) {
            if (pow2 > (n + 1) / 2)
                return false;
            pow2 <<= 1;
        }
        if (pow2 - 1 > n)
            break;

        /* Iterate b starting from 2; stop when the repunit sum exceeds n. */
        for (seq_t b = 2;; ++b) {
            seq_t sum = 1;
            seq_t term = 1;
            bool overflow = false;
            for (idx_t t = 1; t < k; ++t) {
                if (term > n / b) {  /* term * b would overflow n */
                    overflow = true;
                    break;
                }
                term *= b;
                if (sum > n - term) {  /* sum + term would exceed n */
                    overflow = true;
                    break;
                }
                sum += term;
            }
            if (overflow || sum > n)  /* for this k, larger bases only increase sum */
                break;

            if (n % sum == 0) {
                seq_t d = n / sum;
                if (d <= b - 1 && d >= 1)
                    return true;
            }
        }
    }
    return false;
}

/* Dynamic array of cached terms.  */
static seq_t *cached   = NULL;
static size_t     cap   = 0;
static size_t     size  = 0;
static uint64_t   last  = 7;   /* last examined integer */

/* Extend the cache to contain at least `n` terms.  */
static void extend(size_t n)
{
    while (size < n) {
        ++last;
        if (last < 8)               /* skip numbers < 8 */
            continue;
        seq_t candidate = (seq_t)last;
        if (isPrimeGeneric(candidate))
            continue;              /* only composites */
        if (!is_brazilian(candidate))
            continue;
        if (size == cap) {
            size_t newcap = cap ? cap << 1 : 128;
            cached = mem_resize(seq_t, cached, newcap);
            cap = newcap;
        }
        cached[size++] = candidate;
    }
}

/* Public function: a(idx_t n) returns the n-th composite Brazilian number.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but shall not be called with n == 0 */
    extend((size_t)n);
    return cached[(size_t)n - 1];
}
