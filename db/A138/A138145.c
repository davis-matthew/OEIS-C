/*
 * A138145  Palindromes formed from the reflected decimal expansion of the
 *          concatenation of 1, 1, 1 and infinite 0's.
 * https://oeis.org/A138145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 111 + 111*10^(n-3) for n ≥ 6 * Base cases: a(1)=1,
 *            a(2)=11, a(3)=111, * a(4)=1111, a(5)=11111
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 811 (2026-11-01)
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
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)11,
        (seq_t)111,
        (seq_t)1111,
        (seq_t)11111
    };
    if (n <= 5) return init[n-1];
    seq_t p = pow10(n - 3);          /* 10^(n-3) */
    return mul_small(p, (unsigned)111) + (seq_t)111;
}
