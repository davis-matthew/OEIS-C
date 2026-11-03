/*
 * A095528  a(n) = 68 written in base n.
 * https://oeis.org/A095528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal representation of 68 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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
    const seq_t N = 68;

    /* Base 1 uses 68 ones */
    if (n == 1) {
        seq_t v = pow10(68);   /* 10^68 */
        v -= 1;                /* 10^68 - 1 */
        v = v / 9;             /* (10^68 - 1)/9 = 111...1 (68 digits) */
        return v;
    }

    /* Compute digits of N in base n */
    seq_t temp = N;
    seq_t digits[64];
    int sz = 0;
    while (temp) {
        digits[sz++] = temp % n;
        temp = temp / n;
    }

    /* Concatenate digits as decimal string in a seq_t */
    seq_t res = 0;
    for (int i = sz - 1; i >= 0; --i) {
        seq_t d = digits[i];
        int k = num_digits(d);          /* number of decimal digits of d */
        seq_t factor = pow10(k);       /* 10^k */
        res = res * factor + d;
    }
    return res;
}
