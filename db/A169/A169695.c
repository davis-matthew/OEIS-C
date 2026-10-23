/*
 * A169695  a(n) = 1 if n is a square, otherwise a(n) = 2.
 * https://oeis.org/A169695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8365, u16 n <= 8365, u32 n <= 8365, u64 n <= 8365,
 *            u128 n <= 8365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169695 – a(n) = 1 if n is a perfect square, otherwise a(n) = 2.
    The function returns 1 for square numbers and 2 for non‑squares.
    Complexity: O(1) per call, using an integer square‑root algorithm. */

#include <stdint.h>   /* For uint64_t definition, if needed by the build. */

static inline idx_t isqrt_u64(idx_t n)
{
    /* Integer square root of n (floor).  Assumes idx_t is 64‑bit unsigned. */
    idx_t bit = ((idx_t)1) << 62;         /* Highest power of 4 ≤ n. */
    while (bit > n) bit >>= 2;            /* Find starting bit. */
    idx_t res = 0;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t s = isqrt_u64(n);
    /* If n is a perfect square, s*s == n else > n. */
    if (s * s == n) {
        return (seq_t)1;
    } else {
        return (seq_t)2;
    }
}
