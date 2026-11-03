/*
 * A232449  The palindromic Belphegor numbers: (10^(n+3)+666)*10^(n+1)+1.
 * https://oeis.org/A232449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 666*10^(n+1)+100^(n+2)+1
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 7, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 467 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    seq_t p10_1 = pow10(n + 1);          /* 10^(n+1)          */
    seq_t p10_2 = pow10(2 * (n + 2));    /* 10^(2(n+2)) = 100^(n+2) */
    seq_t term1 = p10_1 * (seq_t)666;    /* 666*10^(n+1)      */
    return term1 + p10_2 + (seq_t)1;     /* +1                 */
}
