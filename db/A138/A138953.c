/*
 * A138953  Sum of numbers from all substrings of n.
 * https://oeis.org/A138953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{k-1} d[i] * (i+1) * (10^{k-i} - 1) / 9 - n,
 *            where k is the number of decimal digits of n and d[0] is the
 *            most significant digit. This sums all decimal substrings of n
 *            except n itself
 * Range:     u8 n <= 233, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 8973 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    int k = num_digits_idx(n);                /* number of decimal digits */
    seq_t result = 0;
    idx_t temp = n;                      /* copy to extract digits */

    for (int i = 0; i < k; ++i) {        /* i: position from least significant (0‑based) */
        seq_t digit = (seq_t)(temp % 10);/* current digit */
        temp /= 10;

        seq_t pow_val = pow10(i + 1);    /* 10^{i+1} */
        seq_t factor = (pow_val - 1) / 9;/* (10^{i+1}-1)/9, always integer */

        seq_t term = digit * (seq_t)(k - i) * factor;
        result += term;
    }

    result -= (seq_t)n;                  /* exclude the full number itself */
    return result;
}
