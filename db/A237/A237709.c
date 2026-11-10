/*
 * A237709  Number of occurrences of n-th prime power in A188666.
 * https://oeis.org/A237709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = 2*(q(n) - q(n-1)) where q(k) is the
 *            k-th prime power (A000961, q(1)=1); computed by sieve
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9706 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *pp_list = NULL;
static size_t pp_cnt = 0;

static void pp_build(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    unsigned char *pw = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint64_t i, j;
    size_t cnt = 0;
    if (pp_list) free(pp_list);
    pp_list = (uint64_t *)malloc(((size_t)limit + 1) * sizeof(uint64_t));
    for (i = 2; i <= limit; ++i) {
        if (comp[i]) continue;
        for (j = i * i; j <= limit; j += i) comp[j] = 1;
        {
            uint64_t v = i;
            for (;;) {
                pw[v] = 1;
                if (v > limit / i) break;
                v *= i;
            }
        }
    }
    pw[1] = 1;
    for (i = 1; i <= limit; ++i)
        if (pw[i]) pp_list[cnt++] = i;
    pp_cnt = cnt;
    free(comp);
    free(pw);
}

seq_t a(idx_t n)
{
    static uint64_t lim = 0;
    if (n == 1) return (seq_t)1;
    while (pp_cnt < (size_t)n) {
        lim = lim ? lim * 2 : 1024;
        pp_build(lim);
    }
    {
        uint64_t d = pp_list[n - 1] - pp_list[n - 2];
        return (seq_t)2 * (seq_t)d;
    }
}
