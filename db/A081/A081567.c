/*
 * A081567  Second binomial transform of F(n+1).
 * https://oeis.org/A081567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) with initial values a(0) = 1 , a(1) =
 *            3 (OEIS A081567)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Use a 128‑bit unsigned type for intermediate arithmetic
   to avoid signed‑overflow undefined behaviour. */
static inline seq_t mul5_seq_t(seq_t v) {
    /* 5 * v computed in 128 space, then truncated to seq_t */
    return (seq_t)(((__uint128_t)5 * v));
}

seq_t a(idx_t n)
{
    /* Handle the two base cases outright */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Iterative computation of the linear recurrence.
       Two working variables hold consecutive terms. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t next;

    /* Compute successive terms up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* next = 5*a1 - 5*a0
           perform the multiplications in 128‑bit space
           so that signed‑overflow is never UB.      */
        next = (seq_t)(
                   ( (__uint128_t)5 * a1 )
                 - ( (__uint128_t)5 * a0 )
               );

        /* Shift the window one step forward. */
        a0 = a1;
        a1 = next;
    }

    return a1;
}
