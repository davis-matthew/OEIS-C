/*
 * A004942  a(n) = round(n*phi^7), where phi is the golden ratio, A001622.
 * https://oeis.org/A004942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^7) = floor((29*n + 1 + sqrt(845*n^2))/2),
 *            since phi^7 = (29 + 13*sqrt(5))/2
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a004942_isqrt128, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a004942_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 m = (unsigned __int128)n;
    unsigned __int128 r = a004942_isqrt128((unsigned __int128)845 * m * m); /* floor(13*n*sqrt(5)) */
    unsigned __int128 v = (29 * m + 1 + r) / 2;
    return u128_to_seq(v);
}
