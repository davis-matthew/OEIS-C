/*
 * A004839  Numbers that are the sum of at most 10 nonzero 4th powers.
 * https://oeis.org/A004839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve — a(n) is the n-th number v >= 0 whose minimal count of
 *            nonzero 4th powers summing to v is <= 10 (DP over v)
 * Range:     u8 n <= 140, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static uint64_t *A004839_list = NULL;
static uint64_t A004839_len = 0;

static void A004839_build(uint64_t limit)
{
    unsigned char *cnt = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t v, b, k = 0;
    memset(cnt, 255, (size_t)(limit + 1));
    cnt[0] = 0;
    for (v = 1; v <= limit; v++) {
        unsigned char best = 255;
        for (b = 1; b * b * b * b <= v; b++) {
            unsigned char c = cnt[v - b * b * b * b];
            if (c < best) best = c;
        }
        cnt[v] = (best == 255) ? 255 : (unsigned char)(best + 1);
    }
    for (v = 0; v <= limit; v++)
        if (cnt[v] <= 10) k++;
    free(A004839_list);
    A004839_list = (uint64_t *)malloc((size_t)(k ? k : 1) * sizeof(uint64_t));
    A004839_len = 0;
    for (v = 0; v <= limit; v++)
        if (cnt[v] <= 10) A004839_list[A004839_len++] = v;
    free(cnt);
}

seq_t a(idx_t n)
{
    uint64_t limit = 1024;
    while (A004839_len < (uint64_t)n) {
        while (limit < 2 * (uint64_t)n + 1024) limit *= 2;
        A004839_build(limit);
        limit *= 2;
    }
    return (seq_t)A004839_list[n - 1];
}
