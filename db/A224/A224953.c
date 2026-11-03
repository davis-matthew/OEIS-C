/*
 * A224953  Number of ways a digit can be appended or prepended to n and form
 *          a prime.
 * https://oeis.org/A224953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of primes obtained by concatenating a single
 *            decimal digit either before or after n. The function tests all
 *            ten possible prepended digits and all ten appended digits and
 *            counts how many of the resulting numbers are prime. It uses the
 *            helper function is_prime_big which handles arbitr...
 * Range:     u8 n <= 10, u16 n <= 1054, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 103 terms through n = 9905 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* compute decimal length of n */
    uint64_t len = 0;
    uint64_t tmp = n;
    if (tmp == 0) {
        len = 1;
    } else {
        while (tmp) {
            ++len;
            tmp /= 10;
        }
    }

    /* 10^len as seq_t */
    seq_t pow10_val = pow10((idx_t)len);

    seq_t count = 0;

    for (uint64_t d = 0; d < 10; ++d) {
        /* prepend digit d */
        seq_t val_prepend = (seq_t)d * pow10_val + (seq_t)n;
        if (is_prime_big(val_prepend)) {
            ++count;
        }

        /* append digit d */
        seq_t val_append = (seq_t)n * 10 + (seq_t)d;
        if (is_prime_big(val_append)) {
            ++count;
        }
    }

    return count;
}
