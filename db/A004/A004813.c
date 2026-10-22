/*
 * A004813  Numbers that are the sum of 2 positive 11th powers.
 * https://oeis.org/A004813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: merge the sums a^11 + b^11 (1 <= a <= b) in increasing
 *            order, dropping duplicates; a(n) is the n-th distinct sum
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 1574,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAXB 2000

static seq_t P[MAXB + 2];
static idx_t maxb = 0;
static idx_t nb[MAXB + 2];
static unsigned short *ra = NULL, *rb = NULL;
static idx_t cnt = 0, cap = 0;
static idx_t A = 1;
static bool inited = false;
static bool exhausted = false;

static void init_tab(void) {
    P[0] = 0;
    maxb = 0;
    for (idx_t b = 1; b <= MAXB; b++) {
        seq_t p = 1;
        bool ok = true;
        for (int e = 0; e < 11; e++) {
            seq_t q = p * (seq_t)b;
            if (q / (seq_t)b != p) { ok = false; break; }
            p = q;
        }
        if (!ok) break;
        P[b] = p;
        maxb = b;
    }
    for (idx_t a = 0; a <= MAXB + 1; a++) nb[a] = a;
    A = 1;
    inited = true;
}

static bool step(void) {
    bool found = false;
    seq_t best = 0;
    for (idx_t a = 1; a <= A; a++) {
        if (nb[a] > maxb) continue;
        seq_t s = P[a] + P[nb[a]];
        if (s < P[nb[a]]) continue; /* wrapped in fixed width */
        if (!found || s < best) { best = s; found = true; }
    }
    if (!found) return false;
    bool recorded = false;
    for (idx_t a = 1; a <= A; a++) {
        if (nb[a] > maxb) continue;
        seq_t s = P[a] + P[nb[a]];
        if (s < P[nb[a]]) continue;
        if (s == best) {
            if (!recorded) {
                if (cnt == cap) {
                    cap = cap ? cap * 2 : 1024;
                    ra = (unsigned short *)realloc(ra, cap * sizeof(unsigned short));
                    rb = (unsigned short *)realloc(rb, cap * sizeof(unsigned short));
                }
                ra[cnt] = (unsigned short)a;
                rb[cnt] = (unsigned short)nb[a];
                cnt++;
                recorded = true;
            }
            nb[a]++;
        }
    }
    if (nb[A] != A && A < maxb) A++;
    return true;
}

seq_t a(idx_t n) {
    if (!inited) init_tab();
    while (cnt < n && !exhausted) {
        if (!step()) exhausted = true;
    }
    if (n == 0 || n > cnt) return 0;
    return P[ra[n - 1]] + P[rb[n - 1]];
}
