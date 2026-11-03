/*
 * A194350  Numbers covering A000027: a(n) = (1, 1, 2, 5) * A011557(n).
 * https://oeis.org/A194350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-4) (for n > 4) Initial values: a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=5 where a(n) = (1, 1, 2, 5) * A011557(n)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)5;

    /* Determine the base value and its index based on n mod 4 */
    idx_t mod = n % 4;
    seq_t base;
    idx_t base_index;

    switch (mod) {
        case 1: base = (seq_t)1; base_index = 1; break;
        case 2: base = (seq_t)1; base_index = 2; break;
        case 3: base = (seq_t)2; base_index = 3; break;
        default: /* mod == 0 */ base = (seq_t)5; base_index = 4; break;
    }

    /* Compute the exponent for 10^((n - base_index)/4) */
    idx_t exp = (n - base_index) / 4;

    /* 10^exp as a seq_t */
    seq_t pow = pow10(exp);

    /* Return base * 10^exp */
    return seq_mul(base, pow);
}
