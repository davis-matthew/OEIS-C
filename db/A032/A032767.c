/*
 * A032767  a(n) = floor ( n(n+1)(n+2)(n+3) / (n+(n+1)+(n+2)+(n+3)) ).
 * https://oeis.org/A032767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n*(n+1)*(n+2)*(n+3) / (4*n+6) )
 * Range:     u8 n <= 8, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute using 64‑bit intermediate to avoid overflows for all tested n */
    uint64_t nn   = (uint64_t)n;
    uint64_t num  = nn * (nn + 1) * (nn + 2) * (nn + 3);
    uint64_t den  = 4 * nn + 6;
    uint64_t res  = num / den;
    return (seq_t)res;
}
