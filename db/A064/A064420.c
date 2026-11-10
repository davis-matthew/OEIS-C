/*
 * A064420  Numbers k such that A064413(k+1) = k. Probably complete.
 * https://oeis.org/A064420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A064413(k+1) = k, where A064413 is the
 *            EKG sequence (a(1)=1, a(2)=2, then smallest unused number
 *            sharing a factor with the previous term); found by generating
 *            the EKG sequence with a sieve
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *A064420_found = NULL;
static size_t A064420_cnt = 0;
static size_t A064420_cap = 0;
static uint64_t A064420_N = 0;
static bool A064420_done = false;

static void A064420_add(uint64_t k)
{
    if (A064420_cnt == A064420_cap) {
        size_t nc = A064420_cap ? A064420_cap * 2 : 64;
        uint64_t *p = (uint64_t *)realloc(A064420_found, nc * sizeof(uint64_t));
        if (!p) exit(1);
        A064420_found = p;
        A064420_cap = nc;
    }
    A064420_found[A064420_cnt++] = k;
}

static void A064420_run(uint64_t N)
{
    uint64_t M = 4 * N + 100;
    uint32_t *spf = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(M + 2, 1);
    if (!spf || !ptr || !used) exit(1);

    for (uint64_t i = 2; i <= M; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    A064420_cnt = 0;
    used[1] = 1;
    used[2] = 1;
    uint64_t prev2 = 2;            /* a(2) */
    uint64_t prev = 2;
    /* a(2)=2 : check k=1 -> a(2)==1? no */
    for (uint64_t i = 3; i <= N + 1; i++) {
        uint64_t x = prev, best = UINT64_MAX;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            uint64_t c = ptr[p] ? ptr[p] : p;
            while (c <= M && used[c]) c += p;
            ptr[p] = (uint32_t)(c <= M ? c : 0);
            if (c <= M && c < best) best = c;
        }
        if (best == UINT64_MAX) break;
        used[best] = 1;
        prev = best;
        if (best == i - 1) A064420_add(i - 1);
        (void)prev2;
    }
    free(spf);
    free(ptr);
    free(used);
    A064420_N = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A064420_N == 0) A064420_run(1 << 16);
    while (A064420_cnt < (size_t)n && !A064420_done) {
        if (A064420_N >= ((uint64_t)1 << 22)) { A064420_done = true; break; }
        A064420_run(A064420_N * 2);
    }
    if (A064420_cnt < (size_t)n) return (seq_t)0;
    return (seq_t)A064420_found[n - 1];
}
