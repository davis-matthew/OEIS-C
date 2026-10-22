/*
 * A002191  Possible values for sum of divisors of n.
 * https://oeis.org/A002191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve sigma(m) for all m <= V (sigma(m) > m for m > 1, so every
 *            value <= V comes from some m <= V), mark the values <= V, list
 *            the distinct ones in ascending order, and double V until there
 *            are at least n of them
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a002191_vals = NULL;
static size_t a002191_cnt = 0;
static uint64_t a002191_lim = 0;

static void a002191_build(uint64_t V) {
    uint64_t *sig = (uint64_t *)calloc((size_t)V + 1, sizeof(uint64_t));
    unsigned char *mark = (unsigned char *)calloc((size_t)V + 1, 1);
    uint64_t d, k, m;
    size_t c = 0;
    for (d = 1; d <= V; d++)
        for (k = d; k <= V; k += d)
            sig[k] += d;
    for (m = 1; m <= V; m++)
        if (sig[m] <= V) mark[sig[m]] = 1;
    for (m = 1; m <= V; m++)
        if (mark[m]) c++;
    free(a002191_vals);
    a002191_vals = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
    c = 0;
    for (m = 1; m <= V; m++)
        if (mark[m]) a002191_vals[c++] = m;
    a002191_cnt = c;
    a002191_lim = V;
    free(sig);
    free(mark);
}

seq_t a(idx_t n) {
    if (a002191_cnt < n) {
        uint64_t V = a002191_lim ? a002191_lim * 2 : (uint64_t)n * 8;
        if (V < 64) V = 64;
        for (;;) {
            a002191_build(V);
            if (a002191_cnt >= n) break;
            V *= 2;
        }
    }
    return (seq_t)a002191_vals[n - 1];
}
