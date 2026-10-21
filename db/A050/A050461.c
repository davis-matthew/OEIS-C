/*
 * A050461  a(n) = Sum_{d|n, n/d=1 mod 4} d^2.
 * https://oeis.org/A050461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, (n/d) % 4 == 1} d^2 * with offset 1 (a(1) = 1)
 * Range:     u8 n <= 15, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic types are provided by the build.
 * idx_t   – unsigned 64‑bit integer (for the index n)
 * seq_t   – unsigned integer of the required size (for the result) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize_u64.h"

/* ------------------------------------------------------------------ */
/* Simple factorisation routine (trial division). This works fast     */
/* enough for n up to about 10 000 and avoids the unresolved          */
/* reference to any external utility.                                */

/* ------------------------------------------------------------------ */
/* Recursive enumeration of all divisors of n. For each divisor d,    */
/* if (n/d) mod 4 == 1, add d^2 to the running sum.                 */
/* Curr is kept as uint64_t to avoid overflow of the divisor before   */
/* casting to seq_t for the final addition.                           */
/* ------------------------------------------------------------------ */
static void dfs_divisors(size_t pos, uint64_t curr,
                         uint64_t n,
                         const uint64_t *pr,
                         const uint64_t *ex,
                         size_t cnt,
                         seq_t *sum)
{
    if (pos == cnt) {
        uint64_t rem = n / curr;
        if ((rem & 0x3ULL) == 0x1ULL) {          /* rem % 4 == 1 */
            seq_t d2 = (seq_t)(curr * curr);    /* cast to seq_t; overflows wrap on purpose */
            *sum += d2;
        }
        return;
    }

    uint64_t p = pr[pos];
    uint64_t pow = 1ULL;
    for (uint64_t e = 0ULL; e <= ex[pos]; ++e) {
        dfs_divisors(pos + 1, curr * pow, n, pr, ex, cnt, sum);
        pow *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0ULL)                /* safety guard for out‑of‑range index */
        return 0U;

    /* Arrays large enough for any uint64_t factorisation:
       at most 64 distinct prime factors for 64-bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factorize_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs_divisors(0, 1ULL, (uint64_t)n, primes, exps, cnt, &sum);

    return sum;
}
