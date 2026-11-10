/*
 * A064470  The even subsequence of A064413, divided by 2.
 * https://oeis.org/A064470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064413(k)/2 where A064413(k) is the n-th even term of
 *            the EKG sequence (A064413: a(1)=1, a(2)=2, a(m)=smallest unused
 *            number sharing a factor with a(m-1)); computed by simulating the
 *            EKG sequence
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *ekg_cache = NULL;
static size_t ekg_cnt = 0;

static void ekg_build(size_t n)
{
    size_t L = 16 * n + 1000;
    for (;;) {
        uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
        unsigned char *used = (unsigned char *)calloc(L + 1, 1);
        uint32_t *nxt = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
        uint32_t *out = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
        size_t i, j, cnt = 0;
        bool fail = false;
        for (i = 2; i <= L; i++) {
            if (spf[i] == 0) {
                for (j = i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        for (i = 2; i <= L; i++) nxt[i] = (uint32_t)i;
        used[1] = 1;
        uint32_t prev = 1;
        /* handle terms 1, 2 */
        used[2] = 1;
        prev = 2;
        out[cnt++] = 1;           /* 2/2 */
        while (cnt < n) {
            uint32_t x = prev, best = 0;
            while (x > 1) {
                uint32_t p = spf[x];
                while (x % p == 0) x /= p;
                size_t c = nxt[p];
                while (c <= L && used[c]) c += p;
                if (c > L) { fail = true; break; }
                nxt[p] = (uint32_t)c;
                if (best == 0 || c < best) best = (uint32_t)c;
            }
            if (fail) break;
            used[best] = 1;
            prev = best;
            if ((best & 1) == 0) out[cnt++] = best / 2;
        }
        free(spf); free(used); free(nxt);
        if (!fail) {
            free(ekg_cache);
            ekg_cache = out;
            ekg_cnt = cnt;
            return;
        }
        free(out);
        L *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((size_t)n > ekg_cnt) {
        size_t want = (size_t)n;
        if (want < 2 * ekg_cnt) want = 2 * ekg_cnt;
        if (want < 1000) want = 1000;
        ekg_build(want);
    }
    return (seq_t)ekg_cache[n - 1];
}
