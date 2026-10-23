/*
 * A175846  Partial sums of ceiling(n^2/15).
 * https://oeis.org/A175846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(2*n^2 + n + 56)/90)
 * Range:     u8 n <= 21, u16 n <= 142, u32 n <= 5781, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
    unsigned __int128 N = n;
    unsigned __int128 numerator = (N + 1) * ((unsigned __int128)2 * N * N + N + 56);
    return (seq_t)(numerator / 90);
#else
    unsigned long long N = n;
    unsigned long long numerator = (N + 1) * ((unsigned long long)2 * N * N + N + 56);
    return (seq_t)(numerator / 90);
#endif
}
