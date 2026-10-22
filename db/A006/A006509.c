/*
 * A006509  Cald's sequence: a(n+1) = a(n) - prime(n) if that value is
 *          positive and new, otherwise a(n) + prime(n) if new, otherwise 0;
 *          start with a(1)=1.
 * https://oeis.org/A006509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1) = 1; a(n+1) = a(n) - prime(n) if that is positive and not
 *            already in the sequence, else a(n) + prime(n) if not already in
 *            the sequence, else 0. Computed by direct simulation
 * Range:     u8 n <= 27, u16 n <= 3621, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *cv = NULL;      /* cv[i] = a(i+1) */
static idx_t clen = 0, ccap = 0;
static uint64_t *pr = NULL;      /* pr[i] = prime(i+1) */
static idx_t plen = 0, pcap = 0;
static unsigned char *seenb = NULL;
static uint64_t seencap = 0;

static void ensure_seen(uint64_t v) {
    if (v < seencap) return;
    uint64_t nc = seencap ? seencap : 1024;
    while (nc <= v) nc *= 2;
    seenb = (unsigned char *)realloc(seenb, (size_t)nc);
    memset(seenb + seencap, 0, (size_t)(nc - seencap));
    seencap = nc;
}

static bool seen_has(uint64_t v) { return v < seencap && seenb[v]; }

static void seen_add(uint64_t v) { ensure_seen(v); seenb[v] = 1; }

static uint64_t prime_at(idx_t i) { /* 0-based */
    while (plen <= i) {
        if (plen == pcap) {
            pcap = pcap ? pcap * 2 : 1024;
            pr = (uint64_t *)realloc(pr, (size_t)pcap * sizeof(uint64_t));
        }
        uint64_t c = plen ? pr[plen - 1] + 1 : 2;
        for (;; c++) {
            bool ok = true;
            for (idx_t j = 0; j < plen && pr[j] * pr[j] <= c; j++)
                if (c % pr[j] == 0) { ok = false; break; }
            if (ok) break;
        }
        pr[plen++] = c;
    }
    return pr[i];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (clen == 0) {
        ccap = 1024;
        cv = (uint64_t *)malloc((size_t)ccap * sizeof(uint64_t));
        cv[0] = 1;
        clen = 1;
        seen_add(1);
    }
    while (clen < n) {
        if (clen == ccap) {
            ccap *= 2;
            cv = (uint64_t *)realloc(cv, (size_t)ccap * sizeof(uint64_t));
        }
        uint64_t cur = cv[clen - 1];
        uint64_t p = prime_at(clen - 1);
        uint64_t nx = 0;
        if (cur > p && !seen_has(cur - p)) {
            nx = cur - p;
            seen_add(nx);
        } else if (!seen_has(cur + p)) {
            nx = cur + p;
            seen_add(nx);
        }
        cv[clen++] = nx;
    }
    return (seq_t)cv[n - 1];
}
