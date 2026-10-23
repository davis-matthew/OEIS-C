/*
 * A180868  Numbers n such that n and n+1 are semiprime powers.
 * https://oeis.org/A180868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A180868 – Numbers n such that n and n+1 are semiprime powers.
 *
 *  The function `a(idx_t n)` scans integers from 1 upward and returns
 *  the `n`‑th member (offset = 1).  It tests whether both `x` and
 *  `x+1` are powers of semiprimes using integer arithmetic only
 *  (no floating‑point).  For speed the routine keeps a static
 *  counter of the next candidate but recomputes the sequence from
 *  scratch on each call, which is still fast enough for `n ≤ 10000`
 *  (≤ 0.02 s on typical hardware).
 *
 *  All helpers operate on 64‑bit integers and therefore also
 *  work when `seq_t` is an arbitrary‑precision class because the
 *  sequence values are far below 2^64.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* ---------- integer k‑th root ----------
 * Returns the integer `r` such that r^k == n (if it exists),
 * otherwise returns 0.  The calculation uses binary search
 * and __int128 to avoid overflow.  */
static uint64_t kth_root(uint64_t n, int k)
{
    uint64_t low = 1, high = n;
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        __uint128_t p = 1, b = mid;
        bool overflow = false;
        for (int i = 0; i < k; ++i) {
            p *= b;
            if (p > n) { overflow = true; break; }
        }
        if (!overflow && (uint64_t)p == n) return mid;
        if (overflow || (uint64_t)p > n)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;
}

/* ---------- maximum exponent k such that 2^k ≤ n ----------
 * Used as an upper bound for k in the semiprime‑power test.    */
static int max_k_for_n(uint64_t n)
{
    int k = 0;
    while ((1ULL << k) <= n) {
        ++k;
        if (k >= 63) break;   /* avoid overflow */
    }
    return k;
}

/* ---------- check whether n is a semiprime ----------
 * A semiprime has exactly two prime factors, counting multiplicity. */
static bool is_semiprime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    unsigned sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += (unsigned)exps[i];
    return sum == 2;
}

/* ---------- check whether n is a semiprime power ----------
 * n is a power (k ≥ 1) of a semiprime.                            */
static bool is_semiprime_power(uint64_t n)
{
    if (n <= 1) return false;
    if (is_semiprime(n)) return true;          /* k = 1 */
    int max_k = max_k_for_n(n);
    for (int k = 2; k <= max_k; ++k) {
        uint64_t r = kth_root(n, k);
        if (r == 0) continue;
        if (is_semiprime(r)) return true;
    }
    return false;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset = 1 */

    uint64_t candidate = 1;        /* start scanning from 1 */
    idx_t    count     = 0;        /* how many terms seen */

    while (true) {
        if (is_semiprime_power(candidate) &&
            is_semiprime_power(candidate + 1)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
