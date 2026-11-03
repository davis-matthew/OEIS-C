/*
 * A181669  Primes p of the form 6n-1 such that p-1 is a semiprime and p+2 is
 *          prime or prime squared.
 * https://oeis.org/A181669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   gcd_u64, ipow_i32, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime p of the form 6k‑1 such that
   p−1 is a semiprime and p+2 is either prime or a perfect square
   of a prime.  The sequence is A181669.  The function computes each
   term by a simple search; no dynamic allocation is required. */

#include <stdbool.h>
#include <stdint.h>
/* UTILITIES ------------------------------------------------------------ */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

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

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

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

seq_t a(idx_t n)
{
    /* The sequence is offset 1, so a(1) is the first term. */
    if (n == 0) return (seq_t)0; /* Undefined; return 0 for safety */

    idx_t seen = 0;               /* how many terms we have found */
    uint64_t candidate = 5;       /* first prime of the form 6k‑1 */

    for (;; candidate += 6) {
        if (!is_prime_big((seq_t)candidate))
            continue;                                /* not prime */

        uint64_t t = candidate - 1;
        uint64_t spf = smallest_prime_factor(t);
        if (spf == t)                    /* t is prime → not semiprime */
            continue;
        uint64_t other = t / spf;
        if (!is_prime_big((seq_t)other) /* other not prime → not semiprime */
            )
            continue;

        uint64_t x = candidate + 2;
        bool ok = false;
        if (is_prime_big((seq_t)x))      /* p+2 is prime */
            ok = true;
        else {
            seq_t sq = floor_sqrt((seq_t)x);
            uint64_t root = (uint64_t)sq;
            if (root * root == x && is_prime_big((seq_t)root))
                ok = true;                 /* p+2 is a prime square */
        }
        if (!ok)
            continue;

        /* a candidate satisfying all conditions */
        seen++;
        if (seen == n)
            return (seq_t)candidate;
    }
}
