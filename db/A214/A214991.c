/*
 * A214991  Second nearest integer to n*(1+golden ratio).
 * https://oeis.org/A214991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = second nearest integer to n*(1+golden ratio)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = 2n + ⌊nβ⌋ + [ {nβ} < 0.5 ],  β = (√5−1)/2  */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;          /* cast index to seq_t */
    seq_t sq = (seq_t)5 * N * N;   /* sq = 5·n² = floor(5·n²)               */
    seq_t A  = floor_sqrt(sq);    /* A  = [n√5]  (floor of √5·n)          */
    seq_t t  = (A - N) / 2;       /* t  = ⌊nβ⌋  computed via the identity  */
    seq_t S  = 2 * t + N + 1;     /* S  = 2·⌊nβ⌋ + n + 1                  */
    seq_t cond = (sq < S * S) ? 1 : 0;   /* {nβ} < 0.5  iff 5n² < (2·t+n+1)²   */
    return 2 * N + t + cond;     /* 2n + ⌊nβ⌋ + (cond) = a(n)           */
}
