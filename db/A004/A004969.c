/*
 * A004969  a(n) = ceiling(n*phi^14), where phi is the golden ratio, A001622.
 * https://oeis.org/A004969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^14) = floor((843*n +
 *            floor(377*n*sqrt(5)))/2) + 1 for n >= 1, a(0) = 0, using phi^14
 *            = (L(14) + F(14)*sqrt(5))/2 = (843 + 377*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   a4969_isqrt_s, a4969_isqrt_u, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a4969_isqrt_s.h"
#include "../utils/a4969_isqrt_u.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_big()) {
        seq_t k = (seq_t)n * 377;
        seq_t m = k * k * 5;
        seq_t r = a4969_isqrt_s(m);
        seq_t x = (seq_t)n * 843 + r;
        return x / 2 + 1;
    } else {
        unsigned __int128 k = (unsigned __int128)n * 377;
        unsigned __int128 m = k * k * 5;
        unsigned __int128 r = a4969_isqrt_u(m);
        unsigned __int128 x = (unsigned __int128)n * 843 + r;
        return u128_to_seq(x / 2 + 1);
    }
}
