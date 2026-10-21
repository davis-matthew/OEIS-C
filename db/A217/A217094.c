/*
 * A217094  Least index k such that A011540(k) >= 10^n.
 * https://oeis.org/A217094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217094
 * Least index k such that A011540(k) ≥ 10^n
 * Recurrence: a(0) = 2, a(1) = 2
 *               a(n) = 10*a(n-1) - 9*a(n-2) + 9*10^(n-2)   (n ≥ 2)
 * Formula: a(n) = 2 + 10^n – 9^n – (9^n – 1)/8
 * (both forms give the same result; the recurrence is used here for
 *  efficient, overflow‑safe computation on all supported integer sizes)
 */
#include <stddef.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)2;

    seq_t a0 = (seq_t)2;   /* a(n-2)  */
    seq_t a1 = (seq_t)2;   /* a(n-1)  */
    seq_t pow10 = (seq_t)1;/* 10^(n-2) for the first loop iteration (n=2) */

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 10*a1 - 9*a0 + 9*pow10 */
        seq_t term = a1 * (seq_t)10;           /* 10*a(n-1)  */
        term -= a0 * (seq_t)9;                /* -9*a(n-2)  */
        term += pow10 * (seq_t)9;             /* +9*10^(n-2) */
        a0 = a1;       /* shift previous values */
        a1 = term;
        pow10 *= (seq_t)10; /* advance 10^(n-2) to 10^(n-1) for next iteration */
    }
    return a1;
}
