/*
 * A037827  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*4^i
 *          is the base-4 representation of n.
 * https://oeis.org/A037827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = number of indices i with 1 ≤ i ≤ floor(log4(n))
 * such that the i-th base-4 digit of n is greater than or equal to the (i–1)-th.
 * If n is 0 the function returns 0 (out of range of the sequence).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, safe‑guard */
    seq_t count = 0;
    idx_t t = n;                   /* work copy of the input */
    idx_t prev = t % 4;            /* least‑significant digit */
    t /= 4;
    while (t > 0) {
        idx_t cur = t % 4;
        if (cur >= prev) count++;  /* count non‑decreasing step */
        prev = cur;
        t /= 4;
    }
    return count;                  /* fits in seq_t (max 31 for 64‑bit n) */
}
