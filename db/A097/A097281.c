/*
 * A097281  Integer part of the area of integer triangle [A001611(n),
 *          A001611(n+1), A001611(n+2)].
 * https://oeis.org/A097281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            floor(sqrt(s*(s-A001611(n))*(s-A001611(n+1))*(s-A001611(n+2))));
 *            with x=2*F(n)+1, y=2*F(n+1)+1 this is
 *            floor(isqrt((x+y+1)*x*y)/4), since A001611(n+2) =
 *            A001611(n)+A001611(n+1)-1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A097281_isqrt_s(seq_t v) {
    if (v < 2) return v;
    seq_t r = 1;
    seq_t t = v;
    while (t > 0) { t >>= 2; r <<= 1; }
    r = r << 1;
    for (;;) {
        seq_t y = (r + v / r) / 2;
        if (y >= r) break;
        r = y;
    }
    return r;
}

static unsigned __int128 A097281_isqrt_w(unsigned __int128 v) {
    if (v < 2) return v;
    unsigned __int128 r = 1;
    unsigned __int128 t = v;
    while (t > 0) { t >>= 2; r <<= 1; }
    r = r << 1;
    for (;;) {
        unsigned __int128 y = (r + v / r) / 2;
        if (y >= r) break;
        r = y;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t f0 = 0, f1 = 1;
    for (idx_t i = 0; i < n; i++) {
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    /* f0 = F(n), f1 = F(n+1) */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 x = (unsigned __int128)(2 * f0 + 1);
        unsigned __int128 y = (unsigned __int128)(2 * f1 + 1);
        unsigned __int128 P = (x + y + 1) * x * y;
        return (seq_t)(A097281_isqrt_w(P) / 4);
    } else {
        seq_t x = 2 * f0 + 1;
        seq_t y = 2 * f1 + 1;
        seq_t P = (x + y + 1) * x * y;
        return A097281_isqrt_s(P) / 4;
    }
}
