/*
 * A005166  a(0) = 1; a(n) = (1 + a(0)^3 + ... + a(n-1)^3)/n (not always
 *          integral!).
 * https://oeis.org/A005166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1 + a(0)^3 + ... + a(n-1)^3)/n, a(0) = 1 (sum carried as
 *            s(n) = 1 + sum_{k<n} a(k)^3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_big()) {
        seq_t cur = 1;
        seq_t s = 2; /* 1 + a(0)^3 */
        idx_t i;
        for (i = 1; i <= n; i++) {
            cur = s / (seq_t)i;
            s += cur * cur * cur;
        }
        return cur;
    } else {
        /* fixed-width builds: use a 128-bit accumulator so that
           intermediate sums do not overflow for terms that fit */
        unsigned __int128 cur = 1;
        unsigned __int128 s = 2;
        idx_t i;
        for (i = 1; i <= n; i++) {
            cur = s / (unsigned __int128)i;
            s += cur * cur * cur;
        }
        return u128_to_seq(cur);
    }
}
