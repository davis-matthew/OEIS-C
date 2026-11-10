/*
 * A230742  Successive quadruplets 30*q, 30*q-5, 30*q+5, 30*q+10 in
 *          A064413(n).
 * https://oeis.org/A230742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   search. Build the EKG sequence A064413 with a sieve. Find every
 *            place where 30*q, 30*q-5, 30*q+5, 30*q+10 appear as consecutive
 *            terms, and list those four values in order; a(4k+j) is the j-th
 *            value of the k-th run
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *A230742_hits = NULL;
static size_t A230742_cnt = 0;

static void A230742_build(size_t N)
{
    size_t M = 4 * N + 100;
    uint32_t *spf = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    uint32_t *ptr = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(M + 1, 1);
    uint32_t *s = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    size_t i, j;
    for (i = 2; i <= M; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            ptr[i] = (uint32_t)i;
        }
    }
    s[0] = 1; s[1] = 2;
    used[1] = used[2] = 1;
    for (i = 2; i < N; i++) {
        uint32_t x = s[i - 1], best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t c;
            while (x % p == 0) x /= p;
            c = ptr[p];
            while (c <= M && used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        if (best > M) { N = i; break; }
        used[best] = 1;
        s[i] = best;
    }
    free(A230742_hits);
    A230742_hits = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    A230742_cnt = 0;
    for (i = 0; i + 3 < N; i++) {
        uint32_t v = s[i];
        if (v >= 30 && v % 30 == 0 && s[i + 1] == v - 5 &&
            s[i + 2] == v + 5 && s[i + 3] == v + 10)
            A230742_hits[A230742_cnt++] = v;
    }
    free(spf); free(ptr); free(used); free(s);
}

seq_t a(idx_t n)
{
    static size_t N = 0;
    size_t k = (size_t)(n / 4);
    unsigned r = (unsigned)(n % 4);
    seq_t v;
    while (A230742_cnt <= k) {
        N = N ? N * 2 : 4096;
        A230742_build(N);
    }
    v = (seq_t)A230742_hits[k];
    if (r == 1) return v - 5;
    if (r == 2) return v + 5;
    if (r == 3) return v + 10;
    return v;
}
