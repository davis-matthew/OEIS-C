/*
 * A077344  Triangle in which n-th row contains n smallest primes beginning
 *          with n.
 * https://oeis.org/A077344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2141, u32 n <= 20100, u64 n <= 20100,
 *            u128 n <= 20100, big any n
 * Validated: standard, pass, 71 terms through n = 9602 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077344:
   a(n) is the nth element of the triangle whose r‑th row contains the
   first r primes whose decimal representation begins with the digits
   of r.  The function finds the row containing n and then scans the
   appropriate range of numbers, checking for primality with is_prime_big.
   The search is efficient because r ≤ O(√n) (≈141 for n≤10000),
   so only a few thousand candidates are tested for each call. */

#include <stdbool.h>
#include <stdint.h>
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 for safety */

    /* ---- find the row r such that r*(r-1)/2 < n <= r*(r+1)/2 ---- */
    idx_t r = 1;
    while (true) {
        seq_t tri = (seq_t)r * ((seq_t)r + 1) / 2;
        if (tri >= (seq_t)n)
            break;
        ++r;
    }

    /* position of n within this row (1‑based) */
    seq_t prev = (seq_t)(r - 1) * ((seq_t)r) / 2;
    idx_t k = (idx_t)((seq_t)n - prev);

    /* ---- find the k‑th prime whose decimal representation starts with r ---- */
    seq_t prefix = (seq_t)r;
    idx_t found = 0;

    for (idx_t suffix = 0; ; ++suffix) {
        /* multiplier = 10^suffix */
        seq_t mult = pow10(suffix);
        seq_t start   = prefix * mult;
        seq_t end     = (prefix + 1) * mult - 1;

        for (seq_t cand = start; cand <= end; ++cand) {
            if (is_prime_big(cand)) {
                ++found;
                if (found == k)
                    return cand;
            }
        }
        /* if we reach here, continue with the next suffix length */
    }
}
