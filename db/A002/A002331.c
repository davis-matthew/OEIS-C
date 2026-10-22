/*
 * A002331  Values of x in the solution to p = x^2 + y^2, x <= y, with prime p
 *          = A002313(n).
 * https://oeis.org/A002331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: sieve primes p = 2 or p == 1 (mod 4) to get p = A002313(n); then a(n) = the x with p = x^2 + y^2, x <= y, found by a two-pointer search */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a2331_pr = NULL;
static size_t a2331_cnt = 0;
static uint64_t a2331_lim = 0;

static void a2331_ensure(size_t need) {
    while (a2331_cnt < need) {
        uint64_t lim = a2331_lim ? a2331_lim * 2 : 4096;
        unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
        uint64_t i, j;
        size_t c = 0, cap = 1024;
        uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
        memset(comp, 0, (size_t)lim + 1);
        for (i = 2; i <= lim; i++) {
            if (comp[i]) continue;
            if (i * i <= lim)
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
            if (i == 2 || i % 4 == 1) {
                if (c == cap) {
                    cap *= 2;
                    pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
                }
                pr[c++] = i;
            }
        }
        free(comp);
        free(a2331_pr);
        a2331_pr = pr;
        a2331_cnt = c;
        a2331_lim = lim;
    }
}

seq_t a(idx_t n) {
    uint64_t p, x, y, s;
    a2331_ensure((size_t)n);
    p = a2331_pr[n - 1];
    y = 0;
    while ((y + 1) * (y + 1) <= p) y++;
    x = 1;
    while (x <= y) {
        s = x * x + y * y;
        if (s == p) return (seq_t)x;
        if (s < p) x++;
        else y--;
    }
    return (seq_t)0;
}
