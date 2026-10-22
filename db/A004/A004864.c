/*
 * A004864  Numbers that are the sum of at most 2 positive 7th powers.
 * https://oeis.org/A004864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search - merge the sums i^7 + j^7 (0 <= i <= j) in increasing
 *            order, skipping duplicates
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 292, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_arr.h"

static seq_t *res = 0;
static idx_t rcount = 0, rcap = 0;
static seq_t *pw = 0;
static idx_t *nxt = 0;
static idx_t pcap = 0, rows = 0;

static void ensure_pow(idx_t k) {
    if (k < pcap) return;
    idx_t nc = pcap ? pcap : 16;
    while (nc <= k) nc *= 2;
    pw = grow_arr(pw, pcap, nc);
    nxt = (idx_t *)realloc(nxt, nc * sizeof(idx_t));
    for (idx_t i = pcap; i < nc; i++) {
        seq_t b = (seq_t)i;
        seq_t sq = b * b;
        seq_t v = sq * sq;
        v = v * sq;
        v = v * b;
        pw[i] = v;
        nxt[i] = 0;
    }
    pcap = nc;
}

static void step(void) {
    ensure_pow(rows + 1);
    /* candidate from starting a new row */
    idx_t bestj = rows;
    seq_t best = pw[rows] + pw[0];
    for (idx_t j = 0; j < rows; j++) {
        if (nxt[j] <= j) {
            seq_t c = pw[j] + pw[nxt[j]];
            if (c < best || (c == best && bestj == rows)) {
                best = c;
                bestj = j;
            }
        }
    }
    if (bestj == rows) {
        nxt[rows] = 1;
        rows++;
    } else {
        nxt[bestj]++;
    }
    if (rcount == 0 || res[rcount - 1] != best) {
        if (rcount >= rcap) {
            idx_t nc = rcap ? rcap * 2 : 64;
            res = grow_arr(res, rcap, nc);
            rcap = nc;
        }
        res[rcount++] = best;
    }
}

seq_t a(idx_t n) {
    while (rcount < n) step();
    return res[n - 1];
}
