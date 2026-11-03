/*
 * A036517  Smallest triangular number containing exactly n 0's.
 * https://oeis.org/A036517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 0 a(2) = 300 a(4) = 300700 a(odd n ≥ 3) = 2·10^(n+1) +
 *            10^((n+1)/2) a(even n ≥ 6) = 5·10^(n+1) + 5·10^(n/2) (All powers
 *            of 10 computed by pow10(). Multiplication by a small integer is
 *            done with mul_small().)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 0 (by definition of the sequence) */
    if (n == 1) return (seq_t)0;

    /* Special small cases */
    if (n == 2) {
        /* 300 = 3·10^2 */
        return mul_small(pow10(2), (unsigned)3);
    }
    if (n == 4) {
        /* 300700 = 3·10^5 + 7·10^2 */
        seq_t p5 = mul_small(pow10(5), (unsigned)3);
        seq_t p2 = mul_small(pow10(2), (unsigned)7);
        return p5 + p2;
    }

    /* For odd n ≥3 */
    if ((n & 1) == 1) {
        seq_t p1 = mul_small(pow10(n + 1), (unsigned)2);
        seq_t p2 = pow10((n + 1) / 2);
        return p1 + p2;
    }

    /* For even n ≥6 */
    /* a(n) = 5·10^(n+1) + 5·10^(n/2) */
    seq_t p1 = mul_small(pow10(n + 1), (unsigned)5);
    seq_t p2 = mul_small(pow10(n / 2), (unsigned)5);
    return p1 + p2;
}
