/*
 * A003511  A Beatty sequence: floor( n * (1 + sqrt(3))/2 ).
 * https://oeis.org/A003511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + floor(sqrt(3*n^2)))/2), equivalent to
 *            floor(n*(1+sqrt(3))/2)
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a003511_isqrt3, a003511_isqrt3_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003511_isqrt3.h"
#include "../utils/a003511_isqrt3_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n < ((idx_t)1 << 61)) {
        unsigned __int128 m = a003511_isqrt3((uint64_t)n);
        uint64_t r = (uint64_t)(((unsigned __int128)n + m) / 2u);
        return (seq_t)r;
    }
    return ((seq_t)n + a003511_isqrt3_seq(n)) / 2;
}
