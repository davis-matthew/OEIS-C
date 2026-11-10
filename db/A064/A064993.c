/*
 * A064993  Values for which A065358 is smaller than any preceding term.
 * https://oeis.org/A064993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A065358(k) < min(A065358(0..k-1)), where
 *            A065358(k) = Sum_{j=1..k} (-1)^pi(j); found by sieve and scan
 *            with a doubling limit
 * Range:     u8 n <= 2, u16 n <= 398, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 924 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *rec_list = NULL;
static uint64_t rec_cnt = 0;
static uint64_t rec_limit = 0;

static void recs_compute(uint64_t L)
{
    unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
    if (!comp) exit(1);
    for (uint64_t p = 2; p * p <= L; ++p)
        if (!comp[p])
            for (uint64_t j = p * p; j <= L; j += p)
                comp[j] = 1;
    free(rec_list);
    uint64_t cap = 1024;
    rec_list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!rec_list) exit(1);
    rec_cnt = 0;
    int64_t s = 0, mn = 0;
    uint64_t pi = 0;
    for (uint64_t k = 1; k <= L; ++k) {
        if (k >= 2 && !comp[k]) pi++;
        s += (pi & 1) ? -1 : 1;
        if (s < mn) {
            mn = s;
            if (rec_cnt == cap) {
                cap *= 2;
                rec_list = (uint64_t *)realloc(rec_list, cap * sizeof(uint64_t));
                if (!rec_list) exit(1);
            }
            rec_list[rec_cnt++] = k;
        }
    }
    free(comp);
    rec_limit = L;
}

seq_t a(idx_t n)
{
    if (rec_limit == 0) recs_compute(1u << 16);
    while ((uint64_t)n > rec_cnt)
        recs_compute(rec_limit * 2);
    return (seq_t)rec_list[n - 1];
}
