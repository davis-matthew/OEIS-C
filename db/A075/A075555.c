/*
 * A075555  Smallest prime p such that p+n is a square, or 0 if no such p
 *          exists.
 * https://oeis.org/A075555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   smallest prime p such that p + n is a perfect square; 0 if none
 *            exists
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9883 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, safety guard */

    seq_t n_seq = (seq_t)n;

    /* Detect known zeros: n = m^2 with (2*m+1) composite (odd) */
    seq_t m = floor_sqrt(n_seq);
    if (m * m == n_seq)
    {
        seq_t k = (seq_t)2 * m + 1;
        if (!is_prime_big(k))
            return (seq_t)0;
    }

    /* Start searching from the smallest integer x with x^2 > n+1 (so that p >= 2) */
    seq_t x = floor_sqrt(n_seq + (seq_t)1) + (seq_t)1;

    /* Upper bound on search to avoid infinite loops (conservative for n ≤ 10000) */
    const seq_t MAX_EXTRA = (seq_t)1000000;
    seq_t limit = n_seq + MAX_EXTRA;

    for (; x <= limit; ++x)
    {
        seq_t p = x * x - n_seq;
        if (p < (seq_t)2)
            continue;
        if (is_prime_big(p))
            return p;
    }

    /* If no prime found within the bound, return 0 (should not happen for known n). */
    return (seq_t)0;
}
