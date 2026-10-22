/*
 * A001103  Numbers k such that (k / product of digits of k) is 1 or a prime.
 * https://oeis.org/A001103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   search, not a closed form. Terms are generated one digit length
 *            L at a time. For each multiset of nonzero digits with product P,
 *            we keep the k that are arrangements of those digits with P | k
 *            and k/P equal to 1 or a prime. Small cases use pruned
 *            permutations (the trailing digits must be divisible...
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 2060, u64 n <= 9549,
 *            u128 n <= 9549, big any n
 * Helpers:   a1103_arr, a1103_cmpu, a1103_mulmod, a1103_powmod, a1103_qok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/a1103_arr.h"
#include "../utils/a1103_cmpu.h"
#include "../utils/a1103_mulmod.h"
#include "../utils/a1103_powmod.h"
#include "../utils/a1103_qok.h"

typedef struct { uint64_t P; unsigned char c[10]; } a1103_ms;

static uint64_t *a1103_terms = NULL;
static size_t a1103_nt = 0, a1103_cap = 0;
static int a1103_doneL = 0;

static uint64_t *a1103_buf = NULL;
static size_t a1103_nb = 0, a1103_bcap = 0;

static a1103_ms *a1103_mss = NULL;
static size_t a1103_nms = 0, a1103_mcap = 0;

static void a1103_push(uint64_t k) {
    if (a1103_nb == a1103_bcap) {
        a1103_bcap = a1103_bcap ? 2 * a1103_bcap : 256;
        a1103_buf = (uint64_t *)realloc(a1103_buf, a1103_bcap * sizeof(uint64_t));
    }
    a1103_buf[a1103_nb++] = k;
}

static int a1103_cmpms(const void *x, const void *y) {
    uint64_t u = ((const a1103_ms *)x)->P, v = ((const a1103_ms *)y)->P;
    return u < v ? -1 : (u > v ? 1 : 0);
}

/* collect multisets of L nonzero digits that pass necessary conditions */
static void a1103_msrec(int d, int rem, unsigned char *c) {
    int k;
    if (d == 1) {
        int has5, hasEven, v3, sum = 0, j, e;
        uint64_t P = 1;
        c[1] = (unsigned char)rem;
        has5 = c[5] > 0;
        hasEven = c[2] + c[4] + c[6] + c[8] > 0;
        if (has5 && hasEven) return;
        v3 = c[3] + c[6] + 2 * c[9];
        for (j = 1; j <= 9; j++) {
            sum += j * c[j];
            for (e = 0; e < c[j]; e++) P *= (uint64_t)j;
        }
        if (v3 >= 1 && sum % 3) return;
        if (v3 >= 2 && sum % 9) return;
        if (a1103_nms == a1103_mcap) {
            a1103_mcap = a1103_mcap ? 2 * a1103_mcap : 1024;
            a1103_mss = (a1103_ms *)realloc(a1103_mss, a1103_mcap * sizeof(a1103_ms));
        }
        a1103_mss[a1103_nms].P = P;
        memcpy(a1103_mss[a1103_nms].c, c, 10);
        a1103_nms++;
        return;
    }
    for (k = 0; k <= rem; k++) {
        c[d] = (unsigned char)k;
        a1103_msrec(d - 1, rem - k, c);
    }
    c[d] = 0;
}

static int a1103_L;
static uint64_t a1103_P;
static uint64_t a1103_m2[21], a1103_m5[21];
static unsigned char a1103_cnt[10];

static void a1103_perm(int pos, uint64_t s, uint64_t p10) {
    int d;
    if (pos == a1103_L) {
        if (s % a1103_P == 0 && a1103_qok(s / a1103_P)) a1103_push(s);
        return;
    }
    for (d = 1; d <= 9; d++) {
        if (!a1103_cnt[d]) continue;
        {
            uint64_t ns = s + (uint64_t)d * p10;
            int m = pos + 1;
            if (ns % a1103_m2[m] || ns % a1103_m5[m]) continue;
            a1103_cnt[d]--;
            a1103_perm(pos + 1, ns, p10 * 10);
            a1103_cnt[d]++;
        }
    }
}

static void a1103_gen(int L) {
    unsigned char c[10];
    size_t i, j, t;
    memset(c, 0, sizeof c);
    a1103_nms = 0;
    a1103_nb = 0;
    a1103_msrec(9, L, c);
    qsort(a1103_mss, a1103_nms, sizeof(a1103_ms), a1103_cmpms);
    a1103_L = L;
    for (i = 0; i < a1103_nms; i = j) {
        uint64_t P = a1103_mss[i].P, lo = UINT64_MAX, hi = 0, x;
        int av = 0, cv = 0, m;
        double permcost = 0.0, div, rangecost;
        for (j = i; j < a1103_nms && a1103_mss[j].P == P; j++) {
            const unsigned char *cc = a1103_mss[j].c;
            double f = 1.0;
            int pos = 0, d, e;
            for (d = 1; d <= 9; d++)
                for (e = 1; e <= cc[d]; e++) {
                    pos++;
                    f = f * pos / e;
                }
            permcost += f;
            x = a1103_arr(cc, 1);
            if (x < lo) lo = x;
            x = a1103_arr(cc, 0);
            if (x > hi) hi = x;
        }
        x = P;
        while (x % 2 == 0) { x /= 2; av++; }
        while (x % 5 == 0) { x /= 5; cv++; }
        a1103_m2[0] = 1;
        a1103_m5[0] = 1;
        for (m = 1; m <= L; m++) {
            a1103_m2[m] = a1103_m2[m - 1] * (m <= av ? 2 : 1);
            a1103_m5[m] = a1103_m5[m - 1] * (m <= cv ? 5 : 1);
        }
        div = (double)a1103_m2[L] * (double)a1103_m5[L];
        permcost /= div;
        rangecost = (double)((hi - lo) / P) + 1.0;
        if (rangecost <= permcost) {
            uint64_t k = ((lo + P - 1) / P) * P;
            while (k <= hi) {
                uint64_t tt = k, pr = 1;
                bool ok = true;
                while (tt) {
                    uint64_t d = tt % 10;
                    if (!d) { ok = false; break; }
                    pr *= d;
                    if (pr > P) { ok = false; break; }
                    tt /= 10;
                }
                if (ok && pr == P && a1103_qok(k / P)) a1103_push(k);
                if (hi - k < P) break;
                k += P;
            }
        } else {
            a1103_P = P;
            for (t = i; t < j; t++) {
                memcpy(a1103_cnt, a1103_mss[t].c, 10);
                a1103_perm(0, 0, 1);
            }
        }
    }
    qsort(a1103_buf, a1103_nb, sizeof(uint64_t), a1103_cmpu);
    for (i = 0; i < a1103_nb; i++) {
        if (a1103_nt == a1103_cap) {
            a1103_cap = a1103_cap ? 2 * a1103_cap : 1024;
            a1103_terms = (uint64_t *)realloc(a1103_terms, a1103_cap * sizeof(uint64_t));
        }
        a1103_terms[a1103_nt++] = a1103_buf[i];
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while ((idx_t)a1103_nt < n && a1103_doneL < 19) {
        a1103_doneL++;
        a1103_gen(a1103_doneL);
    }
    if ((idx_t)a1103_nt < n) return (seq_t)0;
    return (seq_t)a1103_terms[n - 1];
}
