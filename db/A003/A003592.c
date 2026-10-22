/*
 * A003592  Numbers of the form 2^i*5^j with i, j >= 0.
 * https://oeis.org/A003592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^A022332(n) * 5^A022333(n); the exponent pairs (i,j) are
 *            generated in increasing order of 2^i*5^j by a Hamming-style
 *            two-pointer merge, comparing i + j*log_2(5) on the exponents
 *            only
 * Range:     u8 n <= 20, u16 n <= 67, u32 n <= 244, u64 n <= 928,
 *            u128 n <= 3620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned short *a3592_e2 = NULL;
static unsigned short *a3592_e5 = NULL;
static idx_t a3592_len = 0, a3592_cap = 0;
static idx_t a3592_i2 = 0, a3592_i5 = 0;

static const double a3592_L5 = 2.32192809488736234787;

static double a3592_key(unsigned x, unsigned y) {
    return (double)x + (double)y * a3592_L5;
}

static void a3592_extend(idx_t n) {
    if (n <= a3592_len) return;
    if (n > a3592_cap) {
        idx_t nc = a3592_cap ? a3592_cap : 64;
        while (nc < n) nc *= 2;
        a3592_e2 = (unsigned short *)realloc(a3592_e2, nc * sizeof(unsigned short));
        a3592_e5 = (unsigned short *)realloc(a3592_e5, nc * sizeof(unsigned short));
        a3592_cap = nc;
    }
    if (a3592_len == 0) {
        a3592_e2[0] = 0;
        a3592_e5[0] = 0;
        a3592_len = 1;
    }
    while (a3592_len < n) {
        unsigned x2 = (unsigned)a3592_e2[a3592_i2] + 1, y2 = a3592_e5[a3592_i2];
        unsigned x5 = a3592_e2[a3592_i5], y5 = (unsigned)a3592_e5[a3592_i5] + 1;
        unsigned x, y;
        if (x2 == x5 && y2 == y5) {
            x = x2; y = y2;
            a3592_i2++; a3592_i5++;
        } else if (a3592_key(x2, y2) < a3592_key(x5, y5)) {
            x = x2; y = y2;
            a3592_i2++;
        } else {
            x = x5; y = y5;
            a3592_i5++;
        }
        a3592_e2[a3592_len] = (unsigned short)x;
        a3592_e5[a3592_len] = (unsigned short)y;
        a3592_len++;
    }
}

seq_t a(idx_t n) {
    if (n < 1) n = 1;
    a3592_extend(n);
    unsigned x = a3592_e2[n - 1], y = a3592_e5[n - 1];
    seq_t r = 1;
    unsigned k;
    for (k = 0; k < x; k++) r *= 2;
    for (k = 0; k < y; k++) r *= 5;
    return r;
}
