/*
 * A223175  Smallest prime q such that 2n+1 = p + 8*q for some odd prime p, or
 *          0 if no such prime exists.
 * https://oeis.org/A223175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime q such that 2*n+1 = p + 8*q for some odd
 *            prime p, or 0 if no such prime exists
 * Range:     u8 n <= 934, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9912 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"

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

/* Helper to shrink a seq_t value to idx_t when it fits. */
static inline bool seq_to_idx(seq_t x, idx_t *out)
{
    /* We rely on the fact that seq_t can be compared with integer
       constants.  If the value is too large for idx_t, or equals
       UINT64_MAX, we treat it as too large. */
    if (x <= (seq_t)UINT64_MAX) {
        *out = (idx_t)x;
        return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* 2*n + 1 (always odd) */
    seq_t target = ((seq_t)n) * 2 + 1;

    /* No odd prime p can satisfy the equation for target < 3. */
    if (target < 3)
        return (seq_t)0;

    /* Compute maximum possible q: 8*q <= target-3 => q <= (target-3)/8 */
    seq_t diff = target - 3;
    seq_t max_q_seq = diff / 8;

    /* Convert to idx_t if possible; otherwise there's no feasible q. */
    idx_t max_q;
    if (!seq_to_idx(max_q_seq, &max_q))
        return (seq_t)0;   /* target so large that q would not fit in idx_t */

    for (idx_t q = 2; q <= max_q; ++q) {
        /* q must be prime. */
        if (!is_prime(q))
            continue;

        /* Corresponding p = target - 8*q. */
        seq_t p = target - (seq_t)8 * q;

        /* p must be an odd prime at least 3. */
        if (p < 3)
            continue;

        /* Fast path: p is even and >2, skip immediately. */
        if ((p & 1) == 0 && p != 2)
            continue;

        /* Finally check primality of p. */
        if (is_prime_big(p))
            return (seq_t)q;  /* smallest qualifying q found */
    }

    /* No suitable prime q. */
    return (seq_t)0;
}
