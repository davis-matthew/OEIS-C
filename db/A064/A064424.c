/*
 * A064424  Record high values in A064413.
 * https://oeis.org/A064424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th record (new maximum) of the EKG sequence A064413,
 *            computed by simulating the EKG sequence with a sieve;
 *            A064413(1)=1, A064413(2)=2, then smallest unused number sharing
 *            a factor with the previous term
 * Range:     u8 n <= 31, u16 n <= 2458, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 70 terms through n = 9146 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *rec_vals = NULL;
static size_t rec_cnt = 0;

/* Simulate EKG with values bounded by L; returns false if bound exceeded
   before collecting `need` records. */
static bool ekg_records_run(size_t need, uint32_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc((size_t)L + 2, 1);
    uint32_t *nxt = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t *recs = (uint64_t *)malloc(need * sizeof(uint64_t));
    size_t cnt = 0;
    bool ok = true;
    uint32_t i, j;

    if (!spf || !used || !nxt || !recs) exit(1);

    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }

    recs[cnt++] = 1;
    used[1] = 1;
    if (need > 1) {
        uint32_t prev = 2, maxv = 1;
        used[2] = 1;
        recs[cnt++] = 2;
        maxv = 2;
        while (cnt < need) {
            uint32_t x = prev, best = 0;
            while (x > 1) {
                uint32_t p = spf[x];
                uint32_t c;
                while (x % p == 0) x /= p;
                c = nxt[p] ? nxt[p] : p;
                while (c <= L && used[c]) c += p;
                nxt[p] = c;
                if (c <= L && (best == 0 || c < best)) best = c;
            }
            if (best == 0) { ok = false; break; }
            used[best] = 1;
            prev = best;
            if (best > maxv) {
                maxv = best;
                recs[cnt++] = best;
            }
        }
    }

    if (ok) {
        free(rec_vals);
        rec_vals = recs;
        rec_cnt = cnt;
    } else {
        free(recs);
    }
    free(spf);
    free(used);
    free(nxt);
    return ok;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((size_t)n > rec_cnt) {
        size_t need = (size_t)n;
        uint32_t L = 1u << 16;
        while ((uint64_t)L < 40 * (uint64_t)need) L <<= 1;
        while (!ekg_records_run(need, L)) L <<= 1;
    }
    return (seq_t)rec_vals[(size_t)n - 1];
}
