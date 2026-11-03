/*
 * A088621  a(n) = smallest prime which is a concatenation of 1, n, n^2, n^3,
 *          ... n^k for some k, or 0 if no such prime exists.
 * https://oeis.org/A088621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the OEIS sequence A088621.
   For n ≥ 1, return the smallest prime that is a concatenation
   of 1, n, n², …, nᵏ for some k ≥ 1, or 0 if none exists. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
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

static const idx_t MAX_ITER = 2000;      /* safe upper bound on k for n up to 10⁴ */

seq_t a(idx_t n)
{
    /* Even n or n divisible by 5 always give composite concatenations: */
    if ((n & 1ULL) == 0ULL || (n % 5ULL) == 0ULL)
        return (seq_t)0;

    seq_t cur   = (seq_t)1;      /* current concatenation (starts with '1')   */
    seq_t power = (seq_t)n;      /* nᵏ, initially k = 1                        */

    for (idx_t k = 1; k <= MAX_ITER; ++k)
    {
        /* Append nᵏ to cur */
        idx_t d = (idx_t)num_digits(power);        /* digits of nᵏ */
        seq_t shift = pow10(d);                   /* 10ᵈ          */
        cur = cur * shift + power;                /* cur = cur*10ᵈ + nᵏ */

        if (is_prime_big(cur))
            return cur;                          /* smallest prime found */

        /* Prepare for next power */
        power = power * (seq_t)n;
    }
    return (seq_t)0;                              /* no prime within limit */
}
