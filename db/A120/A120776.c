/*
 * A120776  Composite numbers k such that k+d+1 is prime for all divisors d of
 *          k greater than 1.
 * https://oeis.org/A120776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 926 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for OEIS sequence A120776:
   The sequence consists of composite integers k for which
   k + d + 1 is prime for every divisor d of k that is > 1.
   The implementation iterates over composite numbers, factors each
   candidate using the provided 64‑bit trial‑division routine
   `factor_u64`, generates all divisors recursively and checks the
   primality condition on the fly.  Results are memoised in a
   static array, so calls in arbitrary order are handled correctly. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions supplied by the build environment */
#include <stddef.h>
#include <limits.h>
#include "../utils/factor_u64.h"
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

static bool satisfies_helper(uint64_t k,
                             const uint64_t *pr,
                             const uint64_t *ex,
                             size_t pf_cnt,
                             size_t idx,
                             uint64_t cur)
{
    if (idx == pf_cnt) {
        if (cur > 1) {
            seq_t test = (seq_t)(k + cur + 1ULL);
            if (!isPrimeGeneric(test))
                return false;
        }
        return true;
    }
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        if (!satisfies_helper(k, pr, ex, pf_cnt, idx + 1, cur * mult))
            return false;
        mult *= pr[idx];
    }
    return true;
}

static bool satisfies(uint64_t k)
{
    /* k must be composite */
    if (isPrimeGeneric((seq_t)k))
        return false;

    /* Quick necessary checks: 2k+1 and k + p + 1, where p is a prime factor */
    if (!isPrimeGeneric((seq_t)(2 * k + 1ULL)))
        return false;

    /* Factorise k */
    uint64_t pr[20], ex[20];
    size_t pf_cnt = 0;
    factor_u64(k, pr, ex, &pf_cnt);

    /* Smallest divisor >1 is pr[0] */
    if (!isPrimeGeneric((seq_t)(k + pr[0] + 1ULL)))
        return false;

    /* Generate all divisors recursively and test */
    return satisfies_helper(k, pr, ex, pf_cnt, 0, 1ULL);
}

seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;
    static size_t len     = 0;
    static size_t cap     = 0;
    static uint64_t last  = 7;        /* last composite examined */

    while (len < (size_t)n) {
        ++last;
        if (isPrimeGeneric((seq_t)last))
            continue;                 /* skip primes */

        if (satisfies(last)) {
            if (len == cap) {
                cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, cap);
                if (!tmp) abort();   /* out of memory */
                terms = tmp;
            }
            terms[len++] = (seq_t)last;
        }
    }
    return terms[n - 1];
}
