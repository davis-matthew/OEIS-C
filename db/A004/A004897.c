/*
 * A004897  Numbers that are the sum of at most 2 nonzero 10th powers.
 * https://oeis.org/A004897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k-way merge of the sorted rows i^10 + j^10 (0 <= i <=
 *            j), keeping only distinct values, so a(n) is the n-th distinct
 *            value
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 53, u64 n <= 3601,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAXP 16384

static seq_t pw[MAXP];
static bool pwo[MAXP];
static idx_t pwn = 0;

static seq_t cur[MAXP];
static bool cinf[MAXP];
static idx_t jj[MAXP];

static void ensure_pw(idx_t i) {
    while (pwn <= i && pwn < MAXP) {
        if (pwn == 0) {
            pw[0] = 0;
            pwo[0] = false;
        } else if (pwo[pwn - 1]) {
            pw[pwn] = 0;
            pwo[pwn] = true;
        } else {
            seq_t p = 1;
            seq_t b = (seq_t)pwn;
            bool o = false;
            int k;
            for (k = 0; k < 10; k++) {
                seq_t q = p * b;
                if (q / b != p) { o = true; break; }
                p = q;
            }
            if (o) { pw[pwn] = 0; pwo[pwn] = true; }
            else { pw[pwn] = p; pwo[pwn] = false; }
        }
        pwn++;
    }
}

static void setrow(idx_t r) {
    idx_t j = jj[r];
    if (j >= MAXP) { cinf[r] = true; return; }
    ensure_pw(j);
    ensure_pw(r);
    if (pwo[r] || pwo[j]) { cinf[r] = true; return; }
    {
        seq_t s = pw[r] + pw[j];
        if (s < pw[j]) { cinf[r] = true; return; }
        cur[r] = s;
        cinf[r] = false;
    }
}

seq_t a(idx_t n) {
    idx_t active = 1, count = 0, r;
    jj[0] = 0;
    setrow(0);
    for (;;) {
        idx_t best = MAXP;
        seq_t v;
        for (r = 0; r < active; r++) {
            if (cinf[r]) continue;
            if (best == MAXP || cur[r] < cur[best]) best = r;
        }
        if (best == MAXP) return 0;
        v = cur[best];
        count++;
        if (count == n) return v;
        for (r = 0; r < active; r++) {
            if (cinf[r] || cur[r] != v) continue;
            jj[r]++;
            setrow(r);
            if (r == active - 1 && active < MAXP) {
                jj[active] = active;
                setrow(active);
                active++;
            }
        }
    }
}
