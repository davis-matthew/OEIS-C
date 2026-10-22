/*
 * A007786  Number of nonintersecting rook paths joining opposite corners of 4
 *          X n board.
 * https://oeis.org/A007786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   G.f.:
 *            x*(x^10-15*x^8+6*x^7+50*x^6-26*x^5-39*x^4+36*x^3-4*x^2-4*x+1) /
 *            ((x^6+2*x^5-9*x^4-5*x^3+15*x^2-8*x+1)*(x^6+2*x^5-7*x^4-3*x^3+7*x^2-4*x+1)),
 *            i.e. a(n) = c(n) + 12*a(n-1) - 54*a(n-2) + 124*a(n-3) -
 *            133*a(n-4) - 16*a(n-5) + 175*a(n-6) - 94*a(n-7) - 69*a(n-8) +
 *            40*a(n-9) + 12*a(n-10) - 4*a(n-11...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const int rec[13] = { 0, 12, -54, 124, -133, -16, 175, -94, -69, 40, 12, -4, -1 };
static const int num[12] = { 0, 1, -4, -4, 36, -39, -26, 50, 6, -15, 0, 1 };

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        computed_up_to = 0;
    }
    if (n <= computed_up_to) return cache[n];

    if (n + 1 > cache_cap) {
        idx_t newcap = cache_cap;
        while (newcap < n + 1) newcap *= 2;
        seq_t *nc = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (idx_t i = 0; i <= computed_up_to; i++) nc[i] = cache[i];
        free(cache);
        cache = nc;
        cache_cap = newcap;
    }

    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t pos = 0, neg = 0;
        if (i < 12) {
            int c = num[i];
            if (c > 0) pos += (seq_t)c;
            else if (c < 0) neg += (seq_t)(-c);
        }
        for (idx_t k = 1; k <= 12 && k <= i; k++) {
            int c = rec[k];
            if (c > 0) pos += (seq_t)c * cache[i - k];
            else neg += (seq_t)(-c) * cache[i - k];
        }
        cache[i] = pos - neg;
    }
    computed_up_to = n;
    return cache[n];
}
