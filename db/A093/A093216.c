/*
 * A093216  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 16.
 * https://oeis.org/A093216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = 10^(2n) - 16*10^n for n≥4; a(1)=0, a(2)=964800,
 *            a(3)=99288000
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
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
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)964800;
    if (n == 3)
        return (seq_t)99288000;

    /* n >= 4 */
    seq_t ten2n = pow10((idx_t)(2ULL * n));
    seq_t tenn  = pow10((idx_t)n);
    seq_t prod  = mul_small(tenn, 16);
    return ten2n - prod;
}
