/*
 * A065057  Partial sums of A064413.
 * https://oeis.org/A065057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A064413(k), a(0) = 0; EKG sequence
 *            A064413(1)=1, A064413(2)=2, then smallest unused number sharing
 *            a factor with the previous term (computed by sieve/search)
 * Range:     u8 n <= 20, u16 n <= 350, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 67 terms through n = 9869 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *ps_sums = NULL;
static size_t ps_len = 0;

static void ps_build(size_t cap)
{
    size_t lim = 4 * cap + 100;
    uint32_t *spf = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(lim + 1, 1);
    if (!spf || !ptr || !used) exit(1);
    for (size_t i = 0; i <= lim; ++i) { spf[i] = 0; ptr[i] = (uint32_t)i; }
    for (size_t i = 2; i <= lim; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= lim; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    if (ps_sums) mem_free(ps_sums);
    ps_sums = mem_new(seq_t, cap + 1);
    ps_sums[0] = (seq_t)0;
    ps_sums[1] = (seq_t)1;
    used[1] = 1;
    uint32_t prev = 1;
    if (cap >= 2) {
        ps_sums[2] = (seq_t)3;
        used[2] = 1;
        prev = 2;
    }
    for (size_t i = 3; i <= cap; ++i) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            uint32_t c = ptr[p];
            while (c <= lim && used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        used[best] = 1;
        prev = best;
        ps_sums[i] = ps_sums[i - 1] + (seq_t)best;
    }
    ps_len = cap;
    free(spf);
    free(ptr);
    free(used);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((size_t)n > ps_len) {
        size_t cap = ps_len * 2;
        if (cap < (size_t)n) cap = (size_t)n;
        if (cap < 1024) cap = 1024;
        ps_build(cap);
    }
    return ps_sums[n];
}
