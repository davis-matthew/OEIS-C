/*
 * A004941  a(n) = round(n*phi^6), where phi is the golden ratio, A001622.
 * https://oeis.org/A004941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^6) = 9*n + round(sqrt(80*n^2)) = 9*n +
 *            floor((isqrt(320*n^2) + 1)/2)
 * Range:     u8 n <= 14, u16 n <= 3652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004941_isqrt128, a004941_isqrt_seq, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a004941_isqrt128.h"
#include "../utils/a004941_isqrt_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n < ((uint64_t)1 << 58)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 s = a004941_isqrt128((unsigned __int128)320 * N * N);
        return u128_to_seq((unsigned __int128)9 * N + (s + 1) / 2);
    } else {
        seq_t N = (seq_t)n;
        seq_t x = N * N * 320;
        seq_t s = a004941_isqrt_seq(x, N * 18);
        return N * 9 + (s + 1) / 2;
    }
}
