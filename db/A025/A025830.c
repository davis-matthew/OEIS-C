/*
 * A025830  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^8)).
 * https://oeis.org/A025830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 6*(n+10)*(1+ ((n+3) mod 4)))/192)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute in a larger integer type to avoid overflow for large n */
    uint64_t nn = (uint64_t)n;                 /* n as unsigned 64‑bit */
    uint64_t n_square = nn * nn;               /* n² */
    uint64_t t2 = 6 * (nn + 10);               /* 6*(n+10) */
    uint64_t mod4 = (nn + 3) & 3ULL;           /* (n+3) mod 4 */
    uint64_t t2_mul = t2 * (1 + mod4);         /* 6*(n+10)*(1+((n+3) mod 4)) */
    uint64_t sum = n_square + t2_mul;          /* numerator */
    uint64_t result = sum / 192ULL;            /* floor division */

    return (seq_t)result;
}
