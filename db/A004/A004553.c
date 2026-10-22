/*
 * A004553  Expansion of sqrt(3) in base 8.
 * https://oeis.org/A004553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, changed
 * Formula:   a(n) = floor(sqrt(3)*8^(n-1)) mod 8. The code computes the
 *            binary digits of sqrt(3) with an exact bit-by-bit integer square
 *            root and groups them three at a time into octal digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq_shl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sq_shl.h"

static uint32_t *sq_Y = NULL, *sq_R = NULL, *sq_T = NULL;
static size_t sq_cap = 0;          /* words allocated in each buffer */
static size_t sq_steps = 0;        /* number of root bits produced so far */
static unsigned char *sq_bits = NULL;
static size_t sq_bitcap = 0;

static void sq_ensure(size_t words) {
    if (words <= sq_cap) return;
    size_t nc = sq_cap ? sq_cap : 16;
    while (nc < words) nc *= 2;
    sq_Y = (uint32_t *)realloc(sq_Y, nc * sizeof(uint32_t));
    sq_R = (uint32_t *)realloc(sq_R, nc * sizeof(uint32_t));
    sq_T = (uint32_t *)realloc(sq_T, nc * sizeof(uint32_t));
    memset(sq_Y + sq_cap, 0, (nc - sq_cap) * sizeof(uint32_t));
    memset(sq_R + sq_cap, 0, (nc - sq_cap) * sizeof(uint32_t));
    memset(sq_T + sq_cap, 0, (nc - sq_cap) * sizeof(uint32_t));
    sq_cap = nc;
}

static void sq_step(unsigned pair) {
    size_t w = sq_steps / 32 + 4;
    size_t i;
    int ge;
    sq_ensure(w + 1);
    /* R = 4R + pair */
    sq_shl(sq_R, w, 2);
    sq_R[0] |= pair;
    /* T = 4Y + 1 */
    memcpy(sq_T, sq_Y, w * sizeof(uint32_t));
    sq_shl(sq_T, w, 2);
    sq_T[0] |= 1u;
    /* compare R >= T */
    ge = 1;
    for (i = w; i-- > 0;) {
        if (sq_R[i] != sq_T[i]) { ge = sq_R[i] > sq_T[i]; break; }
    }
    sq_shl(sq_Y, w, 1);
    if (ge) {
        uint64_t borrow = 0;
        for (i = 0; i < w; i++) {
            uint64_t d = (uint64_t)sq_R[i] - sq_T[i] - borrow;
            sq_R[i] = (uint32_t)d;
            borrow = (d >> 63) & 1u;
        }
        sq_Y[0] |= 1u;
    }
    if (sq_steps >= sq_bitcap) {
        size_t nb = sq_bitcap ? sq_bitcap * 2 : 1024;
        sq_bits = (unsigned char *)realloc(sq_bits, nb);
        sq_bitcap = nb;
    }
    sq_bits[sq_steps] = (unsigned char)(ge ? 1 : 0);
    sq_steps++;
}

seq_t a(idx_t n) {
    size_t need, b;
    unsigned d;
    if (n < 1) return 0;
    need = (size_t)(3 * n - 2);  /* bits 0 .. 3n-3 */
    while (sq_steps < need) sq_step(sq_steps == 0 ? 3u : 0u);
    if (n == 1) return (seq_t)sq_bits[0];
    b = (size_t)(3 * n - 5);
    d = (unsigned)(sq_bits[b] * 4 + sq_bits[b + 1] * 2 + sq_bits[b + 2]);
    return (seq_t)d;
}
