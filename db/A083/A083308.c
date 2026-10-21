/*
 * A083308  a(n) = (4*(n+1)^n + (n-9)^n)/5.
 * https://oeis.org/A083308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4 * (n+1)^n + (n-9)^n) / 5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for unsigned seq_t base and unsigned idx_t exp */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* (n+1)^n */
    seq_t p1 = pow_seq_t((seq_t)(n + 1), n);
    seq_t p1_4 = p1 * 4;   /* 4*(n+1)^n */

    seq_t p2;                /* (n-9)^n or its absolute value */
    if (n < 9) {
        /* (n-9) is negative; use its absolute value */
        idx_t abs_base = 9 - n;            /* |n-9| */
        p2 = pow_seq_t((seq_t)abs_base, n);
        if (n & 1) { /* odd exponent => negative term */
            /* p1_4 should be >= p2 for n >= 0 */
            return (p1_4 - p2) / 5;
        } else {     /* even exponent => add */
            return (p1_4 + p2) / 5;
        }
    } else {
        /* (n-9) is non‑negative */
        idx_t base = n - 9;
        p2 = pow_seq_t((seq_t)base, n);
        return (p1_4 + p2) / 5;
    }
}
