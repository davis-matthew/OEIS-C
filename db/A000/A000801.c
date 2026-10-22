/*
 * A000801  a(n) = Sum_{k = 1..n} floor(2^k / k).
 * https://oeis.org/A000801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(2^k / k)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   a000801_pmod2, a000801_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000801_pmod2.h"
#include "../utils/a000801_width.h"

seq_t a(idx_t n) {
    int w = a000801_width();
    if (w == 0) {
        seq_t sum = 0, p = 1;
        for (idx_t k = 1; k <= n; k++) {
            p += p;
            sum += p / (seq_t)k;
        }
        return sum;
    }
    /* fixed width: compute everything mod 2^128, exact quotient via inverse */
    unsigned __int128 s = 0;
    for (idx_t k = 1; k <= n; k++) {
        uint64_t e = 0, m = k;
        while ((m & 1) == 0) { m >>= 1; e++; }
        uint64_t E = k - e;
        uint64_t r = a000801_pmod2(E, m);
        unsigned __int128 P = (E >= 128) ? 0 : ((unsigned __int128)1 << E);
        unsigned __int128 x = m, inv = m;
        for (int i = 0; i < 8; i++) inv = inv * ((unsigned __int128)2 - x * inv);
        s += (P - (unsigned __int128)r) * inv;
    }
    seq_t res = (seq_t)(uint64_t)s;
    if (w > 64) {
        seq_t sh = 1;
        for (int i = 0; i < 64; i++) sh += sh;
        res += (seq_t)(uint64_t)(s >> 64) * sh;
    }
    return res;
}
