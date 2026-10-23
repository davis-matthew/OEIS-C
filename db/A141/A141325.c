/*
 * A141325  a(n) = A000045(n) + A131531(n+3).
 * https://oeis.org/A141325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141325:
 *  a(n) = A000045(n) + A131531(n+3)
 *  Recurrence: a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)+a(n-5)  (n ≥ 5)
 *  Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=3
 */

#include <stdlib.h>   /* for abort() */

seq_t a(idx_t n)
{
    /* 5‑term sliding window holding a(n‑5) … a(n‑1)  */
    seq_t t0 = 1;   /* a(n‑5) */
    seq_t t1 = 1;   /* a(n‑4) */
    seq_t t2 = 1;   /* a(n‑3) */
    seq_t t3 = 1;   /* a(n‑2) */
    seq_t t4 = 3;   /* a(n‑1) */

    /* Handle indices below the offset explicitly */
    if (n == 0) return t0;
    if (n == 1) return t1;
    if (n == 2) return t2;
    if (n == 3) return t3;
    if (n == 4) return t4;

    /* Compute up to the requested index using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = t4 + t3 - t2 + t1 + t0; /* a(i) */
        t0 = t1;  /* shift values for the next step */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;   /* a(n) */
}
