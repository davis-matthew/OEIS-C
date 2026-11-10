/*
 * A219750  a(n) is formed by concatenating the first n terms of A219749.
 * https://oeis.org/A219750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = concat of A219749(1..n), where A219749(k) =
 *            concat(1^k,12,1^k,2,1^k,12), a term of 3k+5 digits
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 0;
    seq_t p = 1;     /* 10^k */
    seq_t ones = 0;  /* k repeated 1's */
    for (idx_t k = 1; k <= n; k++) {
        p = p * 10;
        ones = ones * 10 + 1;
        seq_t term = ones;
        term = term * 100 + 12;
        term = term * p + ones;
        term = term * 10 + 2;
        term = term * p + ones;
        term = term * 100 + 12;
        seq_t shift = p * p * p * 100000;  /* 10^(3k+5) */
        val = val * shift + term;
    }
    return val;
}
