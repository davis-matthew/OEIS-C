/*
 * A002816  Number of polygons that can be formed from n points on a circle,
 *          no two adjacent.
 * https://oeis.org/A002816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   (n^2-7n+9)a(n) = (n^3-8n^2+18n-21)a(n-1) + 4n(n-5)a(n-2) -
 *            2(n-6)(n^2-5n+3)a(n-3) + (n^2-7n+9)a(n-4) +
 *            (n-5)(n^2-5n+3)a(n-5) for n >= 8, a(1)=1, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=1, a(6)=3, a(7)=23. n^2-7n+9 is always odd, so
 *            fixed-width builds divide by multiplying with its inverse mod
 *            2^w (exact whe...
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   a2816_detect, a2816_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a2816_detect.h"
#include "../utils/a2816_inv.h"

static seq_t *a2816_cache = NULL;
static idx_t a2816_cap = 0;
static idx_t a2816_done = 0;
static int a2816_mode = -1; /* 1 = fixed width, 0 = big */

seq_t a(idx_t n) {
    idx_t i;
    if (n < 1) return 0;
    if (a2816_mode < 0) a2816_mode = a2816_detect();

    if (a2816_cache == NULL) {
        a2816_cap = (n < 1000) ? 1000 : n + 100;
        a2816_cache = (seq_t *)malloc((size_t)(a2816_cap + 1) * sizeof(seq_t));
        a2816_cache[0] = 0;
        a2816_cache[1] = 1;
        a2816_cache[2] = 0;
        a2816_cache[3] = 0;
        a2816_cache[4] = 0;
        a2816_cache[5] = 1;
        a2816_cache[6] = 3;
        a2816_cache[7] = 23;
        a2816_done = 7;
    }
    if (n <= a2816_done) return a2816_cache[n];

    if (n > a2816_cap) {
        idx_t newcap = n + 100;
        seq_t *nc = (seq_t *)malloc((size_t)(newcap + 1) * sizeof(seq_t));
        for (i = 0; i <= a2816_done; i++) nc[i] = a2816_cache[i];
        free(a2816_cache);
        a2816_cache = nc;
        a2816_cap = newcap;
    }

    for (i = a2816_done + 1; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t *c = a2816_cache;
        seq_t coeff = (seq_t)(ni * ni - 7 * ni + 9);
        seq_t c1 = (seq_t)(ni * ni * ni + 18 * ni - 8 * ni * ni - 21);
        seq_t c2 = (seq_t)(4 * ni * (ni - 5));
        seq_t q = (seq_t)(ni * ni - 5 * ni + 3);
        seq_t c3 = (seq_t)(2 * (ni - 6) * q);
        seq_t c5 = (seq_t)((ni - 5) * q);
        seq_t sum = (seq_t)(c1 * c[i - 1]);
        sum = (seq_t)(sum + (seq_t)(c2 * c[i - 2]));
        sum = (seq_t)(sum + (seq_t)(coeff * c[i - 4]));
        sum = (seq_t)(sum + (seq_t)(c5 * c[i - 5]));
        sum = (seq_t)(sum - (seq_t)(c3 * c[i - 3]));
        if (a2816_mode)
            c[i] = (seq_t)(sum * a2816_inv(coeff));
        else
            c[i] = sum / coeff;
    }
    a2816_done = n;
    return a2816_cache[n];
}
