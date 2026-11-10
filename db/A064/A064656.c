/*
 * A064656  Length of n-th run of odd numbers in A064413.
 * https://oeis.org/A064656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of n-th maximal run of consecutive odd terms in
 *            the EKG sequence A064413 (computed by generating the EKG
 *            sequence with a sieve)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A064656_runs = NULL;
static size_t A064656_cnt = 0;

static bool A064656_gen(size_t need, size_t M)
{
    size_t B = 4 * M + 100;
    uint32_t *spf = (uint32_t *)calloc(B + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(B + 1, 1);
    uint32_t *ptr = (uint32_t *)calloc(B + 1, sizeof(uint32_t));
    uint32_t *runs = (uint32_t *)malloc((need + 1) * sizeof(uint32_t));
    if (!spf || !used || !ptr || !runs) exit(1);
    for (size_t i = 2; i <= B; i++) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= B; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    size_t rc = 0;
    uint32_t cur = 1; /* term 1 is odd */
    used[1] = 1;
    used[2] = 1;
    /* term 2 is even: ends run */
    runs[rc++] = cur;
    cur = 0;
    uint32_t prev = 2;
    bool ok = true;
    for (size_t t = 3; t <= M && rc < need; t++) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t p = spf[x];
            while (x % p == 0) x /= p;
            uint32_t c = ptr[p] ? ptr[p] : p;
            while (c <= B && used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        if (best > B) { ok = false; break; }
        used[best] = 1;
        prev = best;
        if (best & 1) cur++;
        else {
            if (cur > 0) { runs[rc++] = cur; cur = 0; }
        }
    }
    if (ok && rc >= need) {
        free(A064656_runs);
        A064656_runs = runs;
        A064656_cnt = rc;
        ok = true;
    } else {
        free(runs);
        ok = false;
    }
    free(spf);
    free(used);
    free(ptr);
    return ok;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    if (need == 0) return (seq_t)0;
    if (need > A064656_cnt) {
        size_t want = need < 64 ? 64 : need;
        size_t M = 12 * want + 200;
        while (!A064656_gen(want, M)) M *= 2;
    }
    return (seq_t)A064656_runs[need - 1];
}
