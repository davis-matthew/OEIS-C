/*
 * A001976  Number of partitions of floor(5n/2)-1 into n nonnegative integers
 *          each no more than 5.
 * https://oeis.org/A001976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^w] of the Gaussian binomial [n+5 choose 5]_q, where w
 *            = floor(5n/2)-1. This equals Sum_{S subset {1..5}} (-1)^|S| *
 *            p5(w - |S|*n - sum(S)), where p5(m) counts the partitions of m
 *            into parts <= 5
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 960, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t w = (5 * n) / 2 - 1;
    seq_t *p = (seq_t *)calloc((size_t)(w + 1), sizeof(seq_t));
    p[0] = 1;
    for (idx_t i = 1; i <= 5; i++)
        for (idx_t j = i; j <= w; j++)
            p[j] += p[j - i];
    seq_t pos = 0, neg = 0;
    for (int mask = 0; mask < 32; mask++) {
        idx_t shift = 0;
        int cnt = 0;
        for (int i = 0; i < 5; i++) {
            if (mask & (1 << i)) {
                cnt++;
                shift += n + (idx_t)(i + 1);
            }
        }
        if (shift > w) continue;
        if (cnt % 2 == 0) pos += p[w - shift];
        else neg += p[w - shift];
    }
    free(p);
    return pos - neg;
}
