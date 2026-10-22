/*
 * A000830  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over G = B_n x S_3 (hyperoctahedral group on
 *            the inputs, S_3 permuting the 3 output bits): a(n) =
 *            (1/(6*2^n*n!)) * Sum_{signed cycle classes g of B_n} |class(g)|
 *            * (8^c + 3*4^co*8^ce + 2*8^c3*2^(c-c3)), where g acting on
 *            {0,1}^n has c cycles, co of odd length, ce of even length,...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a830_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a830_pow2.h"

static int a830_len[64], a830_neg[64], a830_ncyc;
static int a830_n;
static unsigned char *a830_vis;
static seq_t a830_total;

static void a830_leaf(void) {
    int n = a830_n;
    int perm[64];
    unsigned long mask = 0;
    int pos = 0;
    for (int t = 0; t < a830_ncyc; t++) {
        int k = a830_len[t];
        for (int j = 0; j < k; j++)
            perm[pos + j] = pos + (j + 1) % k;
        if (a830_neg[t]) mask |= 1UL << pos;
        pos += k;
    }
    unsigned long D = 1UL << n;
    memset(a830_vis, 0, D);
    idx_t c = 0, co = 0, ce = 0, c3 = 0;
    for (unsigned long s = 0; s < D; s++) {
        if (a830_vis[s]) continue;
        unsigned long x = s;
        idx_t L = 0;
        while (!a830_vis[x]) {
            a830_vis[x] = 1;
            L++;
            unsigned long y = 0;
            for (int i = 0; i < n; i++)
                if (x & (1UL << i)) y |= 1UL << perm[i];
            x = y ^ mask;
        }
        c++;
        if (L % 2) co++; else ce++;
        if (L % 3 == 0) c3++;
    }
    /* class size = 2^n n! / prod_cycles(2k) / prod_groups(m!) */
    seq_t num = a830_pow2((idx_t)n);
    for (int i = 2; i <= n; i++) num *= (seq_t)i;
    seq_t den = 1;
    int t = 0;
    while (t < a830_ncyc) {
        int u = t;
        while (u < a830_ncyc && a830_len[u] == a830_len[t] && a830_neg[u] == a830_neg[t]) u++;
        int m = u - t;
        for (int j = 0; j < m; j++) den *= (seq_t)(2 * a830_len[t]);
        for (int j = 2; j <= m; j++) den *= (seq_t)j;
        t = u;
    }
    seq_t size = num / den;
    seq_t fix = a830_pow2(3 * c)
              + (seq_t)3 * a830_pow2(2 * co + 3 * ce)
              + (seq_t)2 * a830_pow2(3 * c3 + (c - c3));
    a830_total += size * fix;
}

static void a830_rec(int rem, int maxlen, int maxneg) {
    if (rem == 0) { a830_leaf(); return; }
    int top = rem < maxlen ? rem : maxlen;
    for (int len = top; len >= 1; len--) {
        for (int neg = 1; neg >= 0; neg--) {
            if (len == maxlen && neg > maxneg) continue;
            a830_len[a830_ncyc] = len;
            a830_neg[a830_ncyc] = neg;
            a830_ncyc++;
            a830_rec(rem - len, len, neg);
            a830_ncyc--;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    a830_n = (int)n;
    a830_vis = (unsigned char *)malloc((size_t)1 << n);
    a830_total = 0;
    a830_ncyc = 0;
    a830_rec((int)n, (int)n, 1);
    free(a830_vis);
    seq_t g = (seq_t)6 * a830_pow2(n);
    for (idx_t i = 2; i <= n; i++) g *= (seq_t)i;
    return a830_total / g;
}
