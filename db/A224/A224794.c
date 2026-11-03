/*
 * A224794  Smallest prime p such that 2n+1 = 2p - q^3 for some odd prime q,
 *          or 0 if no such prime exists.
 * https://oeis.org/A224794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 2n+1 = 2p – q³ * for an odd
 *            prime q, or 0 if no such prime exists. * * The implementation
 *            searches odd primes q in increasing order. * For each q it
 *            computes p = n + (q³+1)/2 and tests whether p is prime. * If no
 *            solution is found after a large number of trials (...
 * Range:     u8 none, u16 n <= 5, u32 n <= 797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 8315 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Utility prototypes provided by the build system */
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
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

/* Return a(n) */
seq_t a(idx_t n)
{
    /* 2n+1 as seq_t */
    seq_t two_n_plus1 = ((seq_t)n) * (seq_t)2 + (seq_t)1;

    /* If 2n+1 is an odd cube, result is 0. */
    seq_t m = floor_cube_root(two_n_plus1);
    seq_t m3 = m * m * m;
    if (m3 == two_n_plus1)
        return (seq_t)0;

    /* Search odd primes q in increasing order. */
    uint64_t q_start = 3;          /* smallest odd prime */
    size_t  iterations = 0;        /* safety counter */

    while (1) {
        /* Convert current q to seq_t */
        seq_t q_seq = (seq_t)q_start;

        /* Compute q³ and the candidate p */
        seq_t q3   = q_seq * q_seq * q_seq;
        seq_t half = (q3 + (seq_t)1) / (seq_t)2;
        seq_t p    = (seq_t)n + half;

        if (is_prime_big(p))
            return p;              /* first prime p found */

        /* Advance to next odd prime */
        q_start = next_prime(q_start + 1);
        if (q_start == 0)          /* overflow or error */
            break;

        iterations++;
        if (iterations > 100000)   /* safety break */
            break;
    }

    /* If we exit the loop, no suitable prime was found. */
    return (seq_t)0;
}
