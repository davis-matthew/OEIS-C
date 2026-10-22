/*
 * A002180  Values taken by the half-totient function phi(m)/2.
 * https://oeis.org/A002180
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A002202(n)/2, the (n-1)-th distinct value of phi(m)/2
 *            (phi(m) even); computed by a totient sieve over m <= 16*T, which
 *            finds every half-totient value <= T
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache_vals = NULL;
static idx_t cache_cnt = 0;

static void build(idx_t need) {
    uint64_t T = (uint64_t)need * 4 + 64;
    for (;;) {
        /* m/phi(m) < 8 for all m far beyond this range, so phi(m) <= 2T implies m <= 16T */
        uint64_t M = 16 * T;
        uint32_t *ph = (uint32_t *)malloc((size_t)(M + 1) * sizeof(uint32_t));
        unsigned char *mark = (unsigned char *)calloc((size_t)(T + 1), 1);
        uint64_t i, j, cnt = 0;
        for (i = 0; i <= M; i++) ph[i] = (uint32_t)i;
        for (i = 2; i <= M; i++) {
            if (ph[i] == i) {
                for (j = i; j <= M; j += i) ph[j] -= ph[j] / (uint32_t)i;
            }
        }
        for (i = 3; i <= M; i++) {
            uint64_t v = ph[i] / 2;
            if (v <= T) mark[v] = 1;
        }
        for (i = 1; i <= T; i++) if (mark[i]) cnt++;
        if (cnt >= need) {
            free(cache_vals);
            cache_vals = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
            cache_cnt = 0;
            for (i = 1; i <= T; i++) if (mark[i]) cache_vals[cache_cnt++] = i;
            free(ph);
            free(mark);
            return;
        }
        free(ph);
        free(mark);
        T *= 2;
    }
}

seq_t a(idx_t n) {
    idx_t k = n - 1; /* a(2) is the 1st value */
    if (k > cache_cnt) build(k);
    return (seq_t)cache_vals[k - 1];
}
