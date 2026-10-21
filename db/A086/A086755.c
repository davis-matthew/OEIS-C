/*
 * A086755  Sum_{k=1..n} (k(k+1))^2/2.
 * https://oeis.org/A086755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)(n+3)(3*n^2+12*n+10)/30
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS sequence A086755 with 0‑based indexing. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Step 1: capture the index into a seq_t for mixed arithmetic */
    seq_t N = (seq_t)n;            /* n as seq_t */

    /* First factor group (n+1)(n+2)(n+3) */
    seq_t a1 = N + 1;
    seq_t a2 = N + 2;
    seq_t a3 = N + 3;
    seq_t prod1 = a1 * a2 * a3;    /* (n+1)(n+2)(n+3) */

    /* Second factor 3*n^2 + 12*n + 10 */
    seq_t term = (seq_t)3 * N * N   /* 3*n^2          */
                + (seq_t)12 * N     /* + 12*n         */
                + 10;               /* + 10           */

    /* Combine and divide by 30 (exact division) */
    seq_t result = (prod1 * term) / 30;

    return result;
}
