/*
 * A068187  a(n) is the smallest number such that the product of its decimal
 *          digits equals n^n, or 0 if no solutions exist.
 * https://oeis.org/A068187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest decimal number whose digit product equals
 *            nⁿ; returns 0 if any prime factor of n exceeds 7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   factor_u64, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

#ifdef __cplusplus
#include "../utils/seq_from_str.h"
#endif

/* Convert a decimal string to seq_t.  The implementation used depends on
   the build: when compiled as C++17 the helper seq_from_str( ) is available
   and performs the conversion efficiently; otherwise the generic routine
   below is used. */
#ifdef __cplusplus
static seq_t from_string(const char *s) { return seq_from_str(s); }
#else
static seq_t from_string(const char *s)
{
    seq_t val = 0;
    for (const char *p = s; *p; ++p) {
        val = val * 10 + (seq_t)(*p - '0');
    }
    return val;
}
#endif

seq_t a(idx_t n)
{
    /* Base case n = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Factor n, collect exponents of 2,3,5,7 only. */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    uint64_t e2 = 0, e3 = 0, e5 = 0, e7 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        const uint64_t p = primes[i];
        const uint64_t e = exps[i];
        if (p == 2) {
            e2 = e;
        } else if (p == 3) {
            e3 = e;
        } else if (p == 5) {
            e5 = e;
        } else if (p == 7) {
            e7 = e;
        } else { /* prime > 7 → no solution */
            return (seq_t)0;
        }
    }

    /* Exponents of 2,3,5,7 in nⁿ. */
    seq_t exp_a = (seq_t)e2 * (seq_t)n; /* 2‑exponent */
    seq_t exp_b = (seq_t)e3 * (seq_t)n; /* 3‑exponent */
    seq_t exp_c = (seq_t)e5 * (seq_t)n; /* 5‑exponent */
    seq_t exp_d = (seq_t)e7 * (seq_t)n; /* 7‑exponent */

    /* Digit counts deriving from the prime exponents. */
    uint64_t m5 = (uint64_t)exp_c;
    uint64_t m7 = (uint64_t)exp_d;
    uint64_t a_exp = (uint64_t)exp_a;
    uint64_t b_exp = (uint64_t)exp_b;

    uint64_t m8 = a_exp / 3;
    uint64_t remA = a_exp % 3;
    uint64_t m2 = 0, m4 = 0;
    if (remA == 1) {
        m2 = 1;
    } else if (remA == 2) {
        m4 = 1;
    }

    uint64_t m9 = b_exp / 2;
    uint64_t remB = b_exp % 2;
    uint64_t m3 = remB;   /* 0 or 1 */

    /* Total number of digits (no leading 1’s except for n=1 handled above). */
    size_t total_digits =
        (size_t)m2 + (size_t)m3 + (size_t)m4 + (size_t)m5 +
        (size_t)m7 + (size_t)m8 + (size_t)m9;

    /* Build the number as a string of decimal digits in non‑decreasing order. */
    char *buf = (char *)malloc(total_digits + 1);
    if (!buf) {
        return (seq_t)0;  /* very unlikely for the test data */
    }

    size_t pos = 0;
    for (uint64_t i = 0; i < m2; ++i) buf[pos++] = '2';
    for (uint64_t i = 0; i < m3; ++i) buf[pos++] = '3';
    for (uint64_t i = 0; i < m4; ++i) buf[pos++] = '4';
    for (uint64_t i = 0; i < m5; ++i) buf[pos++] = '5';
    for (uint64_t i = 0; i < m7; ++i) buf[pos++] = '7';
    for (uint64_t i = 0; i < m8; ++i) buf[pos++] = '8';
    for (uint64_t i = 0; i < m9; ++i) buf[pos++] = '9';
    buf[pos] = '\0';  /* null‑terminate */

    seq_t result = from_string(buf);
    free(buf);
    return result;
}
