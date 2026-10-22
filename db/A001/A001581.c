/*
 * A001581  Winning moves in Fibonacci nim.
 * https://oeis.org/A001581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve of P-positions of Fibonacci nim: x is a P-position iff no
 *            x - F(k) (F(k) >= 1, F(k) <= x) is a P-position; 0 is one. a(n)
 *            = n-th nonzero P-position
 * Range:     u8 n <= 45, u16 n <= 9525, u32 n <= 9525, u64 n <= 9525,
 *            u128 n <= 9525, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static unsigned char *a001581_isP = NULL;
static uint64_t *a001581_pos = NULL;
static uint64_t a001581_lim = 0;   /* positions [0, lim) are sieved */
static uint64_t a001581_cnt = 0;   /* number of nonzero P-positions found */
static uint64_t a001581_cap = 0;

static void a001581_extend(uint64_t newlim) {
    unsigned char *p = (unsigned char *)realloc(a001581_isP, (size_t)newlim);
    uint64_t x;
    if (!p) return;
    a001581_isP = p;
    for (x = a001581_lim; x < newlim; x++) {
        uint64_t f1 = 1, f2 = 2;
        unsigned char ok = 1;
        if (x == 0) {
            a001581_isP[0] = 1;
            continue;
        }
        while (f1 <= x) {
            if (a001581_isP[x - f1]) { ok = 0; break; }
            {
                uint64_t t = f1 + f2;
                f1 = f2;
                f2 = t;
            }
        }
        a001581_isP[x] = ok;
        if (ok) {
            if (a001581_cnt + 1 >= a001581_cap) {
                uint64_t nc = a001581_cap ? a001581_cap * 2 : 1024;
                uint64_t *q = (uint64_t *)realloc(a001581_pos, (size_t)nc * sizeof(uint64_t));
                if (!q) return;
                a001581_pos = q;
                a001581_cap = nc;
            }
            a001581_cnt++;
            a001581_pos[a001581_cnt] = x;
        }
    }
    a001581_lim = newlim;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (a001581_cnt < n) {
        uint64_t want = (uint64_t)n * 6 + 64;
        uint64_t nl = a001581_lim * 2;
        if (nl < want) nl = want;
        a001581_extend(nl);
    }
    return (seq_t)a001581_pos[n];
}
