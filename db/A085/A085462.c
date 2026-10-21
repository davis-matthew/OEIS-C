/*
 * A085462  Number of 5-tuples (v1,v2,v3,v4,v5) of nonnegative integers less
 *          than n such that v1 <= v4, v1 <= v5, v2 <= v4 and v3 <= v4.
 * https://oeis.org/A085462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)*(3*n+1)*(3*n+2)/120
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 122, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 prod = tn * (tn + 1);
    prod *= (tn * 2 + 1);
    prod *= (tn * 3 + 1);
    prod *= (tn * 3 + 2);
    prod /= 120;
    return (seq_t)prod;
}
