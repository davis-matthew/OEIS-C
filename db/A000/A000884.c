/*
 * A000884  Number of switching networks with C(2,n) acting on the domain and
 *          AG(3,2) acting on the domain.
 * https://oeis.org/A000884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (224*2^(2^n) + 42*4^(2^n) + 8^(2^n) + 448*2^(n + 2^(n-1))
 *            + 92*2^(n + 2^n + 2^(n-1)) + 384*2^n + 168*2^(n+2^n) -
 *            448*2^(2^(n-1)) - 92*2^(2^n + 2^(n-1)))/(1344*2^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a884_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a884_is_fixed.h"

seq_t a(idx_t n) {
    idx_t h = n - 1;
    if (a884_is_fixed()) {
        /* a(n) for n >= 6 exceeds 128 bits, so it never fits a fixed width. */
        if (n > 5) return 0;
        unsigned __int128 x = (unsigned __int128)1 << (1u << h); /* 2^(2^(n-1)) */
        unsigned __int128 N = (unsigned __int128)1 << n;         /* 2^n */
        unsigned __int128 x2 = x * x, x3 = x2 * x;
        unsigned __int128 x4 = x2 * x2, x6 = x3 * x3;
        unsigned __int128 num = 224 * x2 + 42 * x4 + x6 + 448 * N * x
                              + 92 * N * x3 + 384 * N + 168 * N * x2;
        num -= 448 * x;
        num -= 92 * x3;
        return (seq_t)(num / (1344 * N));
    }
    seq_t one = 1;
    seq_t x = one << ((idx_t)1 << h);  /* 2^(2^(n-1)) */
    seq_t N = one << n;                /* 2^n */
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x2 * x2;
    seq_t x6 = x3 * x3;
    seq_t num = x2 * 224;
    num += x4 * 42;
    num += x6;
    num += N * x * 448;
    num += N * x3 * 92;
    num += N * 384;
    num += N * x2 * 168;
    num -= x * 448;
    num -= x3 * 92;
    return num / (N * 1344);
}
