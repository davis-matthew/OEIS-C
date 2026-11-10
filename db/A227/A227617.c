/*
 * A227617  Smallest m such that A100707(m) = n.
 * https://oeis.org/A227617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A100707(m) = n; computed by generating
 *            A100707 (search) until n first appears
 * Range:     u8 n <= 88, u16 n <= 9683, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9039 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A227617_first = 0;   /* first index at which a value occurs */
static unsigned char *A227617_used = 0;
static uint64_t A227617_cap = 0;
static uint64_t A227617_cur = 0, A227617_len = 0, A227617_low = 1;

static void A227617_ensure(uint64_t v)
{
    if (v < A227617_cap) return;
    uint64_t nc = A227617_cap ? A227617_cap : 1024;
    while (nc <= v) nc *= 2;
    uint32_t *nf = (uint32_t *)realloc(A227617_first, nc * sizeof(uint32_t));
    unsigned char *nu = (unsigned char *)realloc(A227617_used, nc);
    if (!nf || !nu) abort();
    A227617_first = nf;
    A227617_used = nu;
    memset(A227617_first + A227617_cap, 0, (nc - A227617_cap) * sizeof(uint32_t));
    memset(A227617_used + A227617_cap, 0, nc - A227617_cap);
    A227617_cap = nc;
}

static void A227617_step(void)
{
    uint64_t M = A227617_cur, k, cand = 0;
    int found = 0;
    for (k = A227617_low; k < M; ++k) {
        if (!A227617_used[k]) {
            cand = M - k;
            if (cand >= A227617_cap || A227617_first[cand] == 0) { found = 1; break; }
        }
    }
    if (!found) {
        for (k = A227617_low; ; ++k) {
            A227617_ensure(M + k);
            if (!A227617_used[k]) {
                cand = M + k;
                if (A227617_first[cand] == 0) break;
            }
        }
    }
    A227617_ensure(cand);
    A227617_used[k] = 1;
    A227617_len++;
    A227617_first[cand] = (uint32_t)A227617_len;
    A227617_cur = cand;
    while (A227617_used[A227617_low]) {
        A227617_low++;
        A227617_ensure(A227617_low);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A227617_first) {
        A227617_ensure(1024);
        A227617_first[1] = 1;
        A227617_cur = 1;
        A227617_len = 1;
    }
    const uint64_t LIMIT = 40000000ULL;
    while (A227617_len < LIMIT) {
        if ((uint64_t)n < A227617_cap && A227617_first[n] != 0)
            return (seq_t)A227617_first[n];
        A227617_step();
    }
    if ((uint64_t)n < A227617_cap && A227617_first[n] != 0)
        return (seq_t)A227617_first[n];
    return 0;
}
