/*
 * A195100  Numbers n such that there are no primes between (n-1)*sqrt(n-1)
 *          and n*sqrt(n).
 * https://oeis.org/A195100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A195100: Numbers n such that no prime lies between (n-1)*sqrt(n-1)
 * and n*sqrt(n).  The n‑th term a(n) is the n‑th integer that satisfies
 * this property.  The algorithm computes the interval borders as
 * floor_sqrt((n-1)^3)+1 and floor_sqrt(n^3) and tests the interval
 * for a prime.  Results are memoised for fast repeated calls. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cached sequence values (allow up to 20 000 terms, plenty for n ≤ 10 000). */
#define CACHE_CAP 20000
static seq_t cache[CACHE_CAP];
static idx_t cache_len = 0;          /* number of terms stored */

/* Current integer being inspected as a candidate. */
static idx_t current_candidate = 1;
/* floor_sqrt((candidate-1)^3) – used to compute the low bound of the next
 * candidate’s interval.  At start it is 0, i.e. floor_sqrt(0). */
static seq_t prev_high = 0;

/* Return true iff there exists a prime p such that low ≤ p ≤ high. */
static bool prime_in_interval(seq_t low, seq_t high)
{
    /* Scan the (small) interval directly; the length is < 200 for all
     * relevant n, so this is inexpensive. */
    for (seq_t x = low; x <= high; ++x)
        if (is_prime_big(x))
            return true;
    return false;
}

/* OEIS A195100 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the offset is 1; a(0) is unused */

    /* Grow the cache until it contains the required term. */
    while (cache_len < n) {
        seq_t cand = (seq_t)current_candidate;

        /* Compute the interval bounds for the current candidate. */
        seq_t cube  = cand * cand * cand;          /* cand³   */
        seq_t high  = floor_sqrt(cube);           /* floor(√cand³) */
        seq_t low   = prev_high + 1;              /* floor(√(cand-1)³)+1 */

        /* Accept the candidate if the interval contains no prime. */
        if (!prime_in_interval(low, high))
            cache[cache_len++] = cand;

        /* Update state for the next candidate. */
        prev_high = high;
        ++current_candidate;
    }
    return cache[n - 1];
}
