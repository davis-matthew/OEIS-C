/*
 * A048345  a(n)^2 is the smallest square containing exactly n 0's.
 * https://oeis.org/A048345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=0; a(2)=10; a(3)=320; a(n)=10*a(n-2) for n>3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    /* Even indices:  a(n) = 10^(n/2) */
    if ((n & 1) == 0) {
        return pow10(n / 2);
    }
    /* Odd indices ≥3 : a(n) = 320 * 10^((n-3)/2) */
    seq_t p = pow10((n - 3) / 2);
    return mul_small(p, 320U);
}
