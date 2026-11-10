/*
 * A064955  Position of n-th prime in A064413.
 * https://oeis.org/A064955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064664(A000040(n)); computed by simulating the EKG
 *            sequence A064413 (a(1)=1, a(2)=2, then smallest unused number
 *            sharing a factor with the previous term) and recording the
 *            position at which the n-th prime first appears
 * Range:     u8 n <= 32, u16 n <= 3618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 173 terms through n = 8171 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

static uint32_t *a064955_pos = NULL;
static size_t a064955_cnt = 0;

static void a064955_run(size_t target)
{
    double t = (double)target;
    size_t pb = (target < 6) ? 20 : (size_t)(t * (log(t) + log(log(t)))) + 10;
    unsigned char *comp = (unsigned char *)calloc(pb + 1, 1);
    size_t i, j, k = 0, p = 2;
    size_t L;
    uint32_t *spf, *nxt;
    unsigned char *used;
    uint32_t *pos;
    uint32_t prev, cur;
    size_t found = 0, step;

    if (!comp) return;
    for (i = 2; i * i <= pb; i++)
        if (!comp[i])
            for (j = i * i; j <= pb; j += i) comp[j] = 1;
    for (i = 2; i <= pb; i++)
        if (!comp[i]) {
            if (++k == target) { p = i; break; }
        }
    free(comp);
    L = 4 * p + 100;

    spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    nxt = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    used = (unsigned char *)calloc(L + 1, 1);
    pos = (uint32_t *)malloc((target + 1) * sizeof(uint32_t));
    if (!spf || !nxt || !used || !pos) {
        free(spf); free(nxt); free(used); free(pos);
        return;
    }
    for (i = 2; i <= L; i++)
        if (spf[i] == 0)
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;

    used[1] = 1; used[2] = 1;
    pos[found++] = 2;
    prev = 2;
    step = 2;
    while (found < target) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t q = spf[x];
            uint32_t c = nxt[q] ? nxt[q] : q;
            while (c <= L && used[c]) c += q;
            nxt[q] = c;
            if (c < best) best = c;
            while (x % q == 0) x /= q;
        }
        if (best > L) break;
        cur = best;
        used[cur] = 1;
        step++;
        prev = cur;
        if (spf[cur] == cur) pos[found++] = (uint32_t)step;
    }
    free(a064955_pos);
    a064955_pos = pos;
    a064955_cnt = found;
    free(spf); free(nxt); free(used);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((size_t)n > a064955_cnt) {
        size_t target = (size_t)n;
        if (target < 2 * a064955_cnt) target = 2 * a064955_cnt;
        if (target < 100) target = 100;
        a064955_run(target);
        if ((size_t)n > a064955_cnt) return (seq_t)0;
    }
    return (seq_t)a064955_pos[n - 1];
}
