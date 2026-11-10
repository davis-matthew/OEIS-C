/*
 * A064426  First differences of A064413.
 * https://oeis.org/A064426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A064413(n+1) - A064413(n), where A064413 is the EKG
 *            sequence: e(1)=1, e(2)=2, e(n) = smallest unused number sharing
 *            a factor with e(n-1)
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9916 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *ekg_t = NULL;
static size_t ekg_len = 0;

static void ekg_build(size_t m)
{
    size_t limit = 4 * m + 100;
    unsigned char *used = (unsigned char *)calloc(limit + 1, 1);
    uint32_t *ptr = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)malloc((m + 2) * sizeof(uint32_t));
    if (!used || !ptr || !t) exit(1);
    t[1] = 1; t[2] = 2;
    used[1] = used[2] = 1;
    for (size_t i = 3; i <= m; i++) {
        uint32_t x = t[i - 1];
        uint32_t best = 0xFFFFFFFFu;
        for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; p++) {
            uint32_t q;
            if ((uint64_t)p * p > x) q = x; else {
                if (x % p) continue;
                q = p;
            }
            while (x % q == 0) x /= q;
            if (ptr[q] == 0) ptr[q] = q;
            while (ptr[q] <= limit && used[ptr[q]]) ptr[q] += q;
            if (ptr[q] < best) best = ptr[q];
            if (x == 1) break;
            p = (q == p) ? p : p;
        }
        used[best] = 1;
        t[i] = best;
    }
    free(used);
    free(ptr);
    free(ekg_t);
    ekg_t = t;
    ekg_len = m;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > ekg_len) {
        size_t m = need < 1024 ? 1024 : need;
        if (m < ekg_len * 2) m = ekg_len * 2;
        ekg_build(m);
    }
    return (seq_t)ekg_t[need] - (seq_t)ekg_t[n];
}
