/*
 * A082942  a(n) = (n^2+1)*(4*n^2+1)*(4*n^2+3).
 * https://oeis.org/A082942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+1)*(4*n^2+1)*(4*n^2+3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned __int128 n128   = (unsigned __int128)(unsigned long long)n;
    unsigned __int128 n_sq   = n128 * n128;
    unsigned __int128 part1  = n_sq + 1;
    unsigned __int128 part2  = (unsigned __int128)4 * n_sq + 1;
    unsigned __int128 part3  = (unsigned __int128)4 * n_sq + 3;
    unsigned __int128 result = part1 * part2 * part3;
    return (seq_t)result;
}
