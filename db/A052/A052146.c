/*
 * A052146  a(n) = floor((sqrt(1+8*n)-3)/2).
 * https://oeis.org/A052146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((sqrt(1+8*n)-3)/2)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9821 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Compute A052146: a(n) = floor((sqrt(1+8*n)-3)/2) for n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* index to sequence type */
    seq_t temp = nn * (seq_t)8 + (seq_t)1;  /* 1 + 8*n  */
    seq_t s    = isqrt_seq(temp);   /* floor(sqrt(1+8*n)) */
    seq_t r    = (s - (seq_t)3) / (seq_t)2; /* (s-3)/2 */
    return r;
}
