/*
 * A076384  Numbers n such that sum of digits in base 6 is a divisor of sum of
 *          prime divisors (A008472).
 * https://oeis.org/A076384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th integer >1 such that the sum of its
 * base‑6 digits divides the sum of its *distinct* prime divisors
 * (OEIS A076384).  This implementation searches sequentially. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

#define MAX_TERMS 200000  /* enough for the first 10 000 listed terms */

/* Test whether a positive integer satisfies the defining condition. */
static bool satisfies_uint64(uint64_t x)
{
    /* Sum of digits of x in base 6 (s6). */
    uint64_t s6 = 0;
    uint64_t t  = x;
    while (t) {
        s6 += t % 6;
        t  /= 6;
    }
    if (s6 == 0) return false; /* never happens for x >= 2 */

    /* Sum of *distinct* prime divisors. */
    uint64_t primes[64], exps[64];   /* enough for any reasonably sized x */
    size_t   cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    uint64_t distinct_sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        distinct_sum += primes[i];

    return (distinct_sum % s6) == 0;
}

/* ------------------------------------------------------------------ */
/* Main routine: returns the nth term (n ≥ 1).                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t cache[MAX_TERMS];
    static idx_t found = 0;          /* number of terms already stored          */
    static uint64_t next = 2;        /* next candidate to test                 */

    while (found < n) {
        if (satisfies_uint64(next)) {
            cache[found++] = (seq_t)next;
        }
        ++next;
        /* Guard against exceeding the cache size – should never happen for the
         * requested range of n. */
        if (found == MAX_TERMS) return 0;
    }
    return cache[n - 1];
}
