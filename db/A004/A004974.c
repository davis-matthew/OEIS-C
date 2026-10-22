/*
 * A004974  a(n) = ceiling(n*phi^19), where phi is the golden ratio, A001622.
 * https://oeis.org/A004974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^19) = 9349*n + floor((isqrt(87403805*n^2) -
 *            9349*n)/2) + 1 for n >= 1, a(0) = 0 (phi^19 = 9349 + phi^-19 and
 *            phi^-19 = (4181*sqrt(5) - 9349)/2, since L(19) = 9349, F(19) =
 *            4181)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r, y;
    int bits = 0;
    if (x < 2) return x;
    y = x;
    while (y) { bits++; y >>= 1; }
    r = (unsigned __int128)1 << ((bits + 1) / 2);
    for (;;) {
        y = (r + x / r) >> 1;
        if (y >= r) break;
        r = y;
    }
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

seq_t a(idx_t n) {
    unsigned __int128 nn, s, k;
    seq_t res;
    if (n == 0) return (seq_t)0;
    nn = (unsigned __int128)n;
    s = isqrt_u128((unsigned __int128)87403805u * nn * nn);
    k = (s - (unsigned __int128)9349u * nn) / 2;  /* floor(n * phi^-19), small */
    res = (seq_t)n * 9349;
    res += (seq_t)(uint64_t)k;
    res += 1;
    return res;
}
