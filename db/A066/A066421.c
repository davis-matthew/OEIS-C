/*
 * A066421  a(n) = least k > 0 such that sigma^(k)(n) + 1 is prime, if such k
 *          exists; otherwise 0, where sigma^(k) denotes application of sigma
 *          k times.
 * https://oeis.org/A066421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 36089, u64 n <= 36089,
 *            u128 n <= 36089, big any n
 * Validated: standard, pass, 116 terms through n = 9715 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k > 0 such that σ^k(n) + 1 is prime,
   otherwise 0.  Uses sigma_of and isPrimeGeneric from utils. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
    if (n == 0) return (seq_t)0; /* safety for indices below offset */

    idx_t cur = n;
    seq_t k = 1;                 /* step counter (k > 0) */

    for (;;) {
        /* Apply σ once.  sigma_of returns a seq_t value. */
        seq_t m = sigma_of(cur);

        /* Candidate to test for primality. */
        seq_t cand = m + (seq_t)1;
        if (isPrimeGeneric(cand))
            return (seq_t)k;

        /* Stop if m cannot be represented as idx_t (overflow). */
        if (m > (seq_t)UINT64_MAX)
            break;

        /* Prepare for next iteration. */
        cur = (idx_t)m;
        k++;
    }

    /* No suitable k found within representable range. */
    return (seq_t)0;
}
