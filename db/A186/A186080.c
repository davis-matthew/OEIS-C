/*
 * A186080  Fourth powers that are palindromic in base 10.
 * https://oeis.org/A186080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0, a(2)=1; for n≥3, a(n) = (10^(n-2)+1)^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 10^e as seq_t (e fits in idx_t, small for n ≤ 10 000) */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* Helper to compute 10^e as a seq_t using repeated multiplication.
   This works for any seq_t (fixed or arbitrary precision). */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    /* n ≥ 3: compute (10^(n-2)+1)^4 */
    seq_t base = pow10_seq(n - 2);
    base += (seq_t)1;
    seq_t sq = base * base;
    return sq * sq;
}
