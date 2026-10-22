/*
 * A006006  Weight distribution of [ 128,29,32 ] 2nd-order Reed-Muller code.
 * https://oeis.org/A006006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   RM(2,7) weight distribution (Sloane-Berlekamp). a(n) = A_{8n};
 *            A_0 = A_128 = 1; for h = 1..3, A_{64 +- 2^(6-h)} = 2^(h(h+1)) *
 *            Prod_{i=0..2h-1} (2^(7-i)-1) / Prod_{i=1..h} (4^i-1); A_64 =
 *            2^29 - 2 - 2*Sum_{h} A_{64-2^(6-h)}; all other terms are 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   rm2_count, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/rm2_count.h"

seq_t a(idx_t n) {
    const int m = 7;
    const long long len = 1LL << m;       /* 128 */
    const long long step = 8;             /* index n corresponds to weight 8n */
    long long w, d;
    int h;
    if (n > (idx_t)(len / step)) return (seq_t)0;
    w = (long long)n * step;
    if (w == 0 || w == len) return (seq_t)1;
    d = w - len / 2;
    if (d < 0) d = -d;
    if (d == 0) {
        unsigned long long total = 1ULL << (1 + m + m * (m - 1) / 2);
        unsigned long long s = 0;
        for (h = 1; h <= m / 2; h++) s += rm2_count(m, h);
        return u128_to_seq((unsigned __int128)(total - 2 - 2 * s));
    }
    for (h = 1; h <= m / 2; h++)
        if (d == (1LL << (m - 1 - h)))
            return u128_to_seq((unsigned __int128)rm2_count(m, h));
    return (seq_t)0;
}
