/*
 * A224526  Number of idempotent 4 X 4 0..n matrices of rank 1.
 * https://oeis.org/A224526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 810, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4 * [ (n+1)^3
 *                       + 3 * Σ_{u=1..n} (⌊n/u⌋ + 1)^2
 *                       + 3 * Σ_{m=1..n} (2m – 1) * (⌊n/m⌋ + 1)
 *                       + n^3 ]
 *
 * This expression counts the number of idempotent 4×4 matrices with
 * entries in {0,…,n} and rank 1.
 */
#include <stdint.h>

/* external linkage function required */
seq_t a(idx_t n)
{
    if (n == 0)    /* sequence is defined for n ≥ 1 */
        return 0;

    seq_t N  = (seq_t)n;          /* cast index to seq_t early        */
    seq_t N1 = N + 1;             /* n + 1                            */

    /* polynomial terms */
    seq_t S0 = N1 * N1 * N1;      /* (n+1)^3                          */
    seq_t S3 = N  * N  * N;       /* n^3                              */

    /* sums over m = 1..n   (the same loop works for both S1 and S2) */
    seq_t S1 = 0;
    seq_t S2 = 0;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t v   = N / m;                /* ⌊n/m⌋        */
        seq_t vp1 = v + 1;                /* ⌊n/m⌋ + 1    */
        S1 += vp1 * vp1;                  /* Σ (⌊n/m⌋+1)^2 */
        seq_t mult = (seq_t)2 * m - 1;     /* (2m – 1)     */
        S2 += mult * vp1;                 /* Σ (2m-1)(⌊n/m⌋+1) */
    }

    /* combine all contributions */
    seq_t T = S0 + 3 * S1 + 3 * S2 + S3;
    return 4 * T;
}
