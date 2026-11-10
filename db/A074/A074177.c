/*
 * A074177  Where records occur in A064413.
 * https://oeis.org/A074177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th record (strict left-to-right
 *            maximum) in the EKG sequence A064413; computed by simulating
 *            A064413 with per-prime pointers to the smallest unused multiple,
 *            doubling the length until n records are found
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 977 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *rec_pos = NULL;
static size_t rec_cnt = 0;

static bool ekg_build(size_t N)
{
    size_t V = 4 * N + 64;
    unsigned char *used = (unsigned char *)calloc(V + 1, 1);
    uint32_t *ptr = (uint32_t *)calloc(V + 1, sizeof(uint32_t));
    uint64_t *rp = (uint64_t *)malloc((N + 2) * sizeof(uint64_t));
    size_t rc = 0;
    bool ok = true;
    if (!used || !ptr || !rp) exit(1);

    uint32_t prev = 2, maxv = 2;
    used[1] = 1; used[2] = 1;
    rp[rc++] = 1;
    rp[rc++] = 2;
    for (size_t i = 3; i <= N; i++) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; p++) {
            if ((uint64_t)p * p > x) p = x;
            if (x % p == 0) {
                while (x % p == 0) x /= p;
                uint32_t c = ptr[p] ? ptr[p] : p;
                while (c <= V && used[c]) c += p;
                ptr[p] = c;
                if (c < best) best = c;
            }
            if (x == 1) break;
        }
        if (best > V) { ok = false; break; }
        used[best] = 1;
        if (best > maxv) { maxv = best; rp[rc++] = i; }
        prev = best;
    }
    if (ok) {
        free(rec_pos);
        rec_pos = rp;
        rec_cnt = rc;
    } else {
        free(rp);
    }
    free(used);
    free(ptr);
    return ok;
}

seq_t a(idx_t n)
{
    static size_t N = 256;
    while (rec_cnt < (size_t)n) {
        if (rec_pos != NULL) N *= 2;
        if (!ekg_build(N)) { N *= 2; ekg_build(N); }
    }
    return (seq_t)rec_pos[n - 1];
}
