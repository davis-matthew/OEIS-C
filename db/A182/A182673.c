/*
 * A182673  a(n) = the smallest n-digit number with exactly 7 divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest n‑digit number of the form p⁶ (p prime), or
 *            0 if no such number exists
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n) {
    /* lower bound L = 10^(n-1) */
    seq_t L = pow10(n - 1);

    /* starting point: 10^floor((n-1)/6) */
    idx_t expLow = (n - 1) / 6;
    seq_t p = pow10(expLow);

    /* step until p⁶ >= L */
    seq_t pPow = ipow(p, (seq_t)6);
    while (pPow < L) {
        ++p;
        pPow = ipow(p, (seq_t)6);
    }

    /* search for the first prime ≥ p with exactly n digits */
    if (p < (seq_t)2) {
        p = (seq_t)2;
    }
    /* ensure p is odd if > 2 */
    if (p > (seq_t)2 && (p & (seq_t)1) == (seq_t)0) {
        ++p;
    }

    while (1) {
        seq_t power = ipow(p, (seq_t)6);
        int digits = num_digits(power);
        if (digits > (int)n) {   /* exceeded the allowed digit count */
            break;
        }
        if (digits == (int)n && is_prime_big(p)) {
            return power;       /* found the smallest prime p */
        }
        p += (seq_t)2;           /* move to the next odd candidate */
    }

    return (seq_t)0;              /* no such number exists */
}
