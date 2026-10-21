/*
 * A206904  n+[nr/s]+[nt/s], where []=floor, r=3, s=sqrt(3), t=1/s.
 * https://oeis.org/A206904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor(n/3)
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    // Assume n >= 1; behavior for n < 1 is undefined
    seq_t nn = (seq_t)n;           // convert index to seq_t type
    seq_t part1 = nn;              // n
    seq_t part2 = floor_sqrt( (seq_t)3 * nn * nn ); // floor(n*sqrt(3)) == floor(sqrt(3*n^2))
    seq_t part3 = nn / 3;          // floor(n/3)
    return part1 + part2 + part3;
}
