/*
 * A004939  a(n) = round(n*phi^4), where phi is the golden ratio, A001622.
 * https://oeis.org/A004939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^4) = floor((7*n + 1 +
 *            floor(sqrt(45*n^2)))/2), since phi^4 = (7+3*sqrt(5))/2
 * Range:     u8 n <= 37, u16 n <= 9561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004939_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004939_isqrt.h"

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 m = a004939_isqrt(45 * nn * nn);
    unsigned __int128 r = (7 * nn + 1 + m) / 2;
    return (seq_t)(uint64_t)r;
}
