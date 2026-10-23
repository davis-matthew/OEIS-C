/*
 * A025814  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A025814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 13*n + 144 + 9*(n+8)*(-1)^n + 4*n*((n+2) mod
 *            3))/216)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the number of partitions of n into parts 2, 6, and 9. */
seq_t a(idx_t n)
{
    /* Use 64‑bit signed arithmetic for the intermediate calculation.
       For n ≤ 10000 the intermediate values are far below 2^63. */
    int64_t i = (int64_t)n;
    int64_t sign = (i & 1) ? -1 : 1;            /* (-1)^n   */
    int64_t part = i + 8;                      /* (n+8)    */
    int64_t mod3 = (i + 2) % 3;                /* ((n+2) mod 3) */

    int64_t val = i * i          /* n^2     */
                + 13 * i         /* 13n     */
                + 144            /* constant */
                + 9 * part * sign
                + 4 * i * mod3;

    return (seq_t)(val / 216); /* floor division */
}
