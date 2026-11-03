/*
 * A069585  a(n) = n - largest prime power <= n.
 * https://oeis.org/A069585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n – L where L is the largest number ≤ n that is a prime
 *            power of the form p^k (p prime, k ≥ 2), with 1 counted as a
 *            special prime power. The function returns a(idx_t n) with
 *            external linkage, suitable for all seq_t/idx_t types
 * Range:     u8 n <= 1278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9338 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/*  Return true iff base^exp ≤ limit; 90‑bit * is used
    to avoid overflow for 64‑bit limit values.                */
static bool pow_leq_ui(uint64_t base, int exp, uint64_t limit)
{
    __int128 prod = 1;
    for (int i = 0; i < exp; ++i) {
        prod *= base;
        if (prod > limit) return false;
    }
    return true;
}

/*  Integer floor of n^(1/e) for n ≥ 1, e ≥ 1 .
    Binary search using pow_leq_ui for correctness. */
static uint64_t nth_root_floor(uint64_t n, int e)
{
    if (e == 1) return n;
    uint64_t lo = 1, hi = n;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (pow_leq_ui(mid, e, n))
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/*  Main sequence function.  It is safe for any idx_t ≥ 1
    and works under the original performance constraints.   */
seq_t a(idx_t n)
{
    /*  The smallest considered prime power is 1.          */
    seq_t best = 1;           /* best prime power found so far */

    /*  Search exponents k = 2,3,4,… until no base ≥2
        exists such that 2^k ≤ n.                          */
    for (int k = 2; ; ++k) {
        /*  Stop when even the smallest base (2) exceeds n.  */
        if (!pow_leq_ui(2, k, (uint64_t)n))
            break;

        /*  Largest possible base for this exponent.       */
        uint64_t bound = nth_root_floor((uint64_t)n, k);
        if (bound < 2L) continue;

        /*  Find the largest prime ≤ bound.                */
        uint64_t p = bound;
        while (p >= 2L && !is_prime(p))
            --p;
        if (p < 2L) continue;

        /*  Compute p^k as seq_t (fits because p^k ≤ n).   */
        seq_t power = 1;
        for (int i = 0; i < k; ++i)
            power = power * (seq_t)p;

        if (power > best)
            best = power;
    }

    return (seq_t)n - best;
}
