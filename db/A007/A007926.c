/*
 * A007926  Some permutation of digits is a factorial number.
 * https://oeis.org/A007926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search over digit lengths L: a(n) runs through, in increasing
 *            order, the L-digit numbers (no leading zero) whose digits are
 *            those of some factorial k! plus extra zeros; a permutation of
 *            these digits, allowing leading zeros, is a factorial
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   next_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/next_perm.h"

static seq_t *terms = 0;
static idx_t nterms = 0, capterms = 0;
static int curL = 0;
static int g_L = 0;

static int cmp_str(const void *x, const void *y) {
    return memcmp(x, y, (size_t)g_L);
}

static void push_term(seq_t v) {
    if (nterms == capterms) {
        idx_t nc = capterms ? capterms * 2 : 256;
        seq_t *nt = (seq_t *)malloc(sizeof(seq_t) * (size_t)nc);
        for (idx_t i = 0; i < nterms; i++) nt[i] = terms[i];
        free(terms);
        terms = nt;
        capterms = nc;
    }
    terms[nterms++] = v;
}

static void gen_length(int L) {
    int sets[64][10];
    int nsets = 0;
    int fd[256];
    int flen = 1;
    fd[0] = 1;
    for (int k = 1; ; k++) {
        int carry = 0;
        for (int i = 0; i < flen; i++) {
            int x = fd[i] * k + carry;
            fd[i] = x % 10;
            carry = x / 10;
        }
        while (carry) { fd[flen++] = carry % 10; carry /= 10; }
        if (flen > L) break;
        int c[10];
        for (int i = 0; i < 10; i++) c[i] = 0;
        for (int i = 0; i < flen; i++) c[fd[i]]++;
        c[0] += L - flen;
        bool dup = false;
        for (int s = 0; s < nsets && !dup; s++) {
            bool same = true;
            for (int i = 0; i < 10; i++) if (sets[s][i] != c[i]) { same = false; break; }
            if (same) dup = true;
        }
        if (!dup) {
            for (int i = 0; i < 10; i++) sets[nsets][i] = c[i];
            nsets++;
        }
    }

    size_t cnt = 0, cap = 1024;
    char *buf = (char *)malloc(cap * (size_t)L);
    char d[256];
    for (int s = 0; s < nsets; s++) {
        int c[10];
        for (int i = 0; i < 10; i++) c[i] = sets[s][i];
        int first = 1;
        while (first < 10 && c[first] == 0) first++;
        if (first == 10) continue;
        c[first]--;
        d[0] = (char)first;
        int p = 1;
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < c[i]; j++) d[p++] = (char)i;
        do {
            if (cnt == cap) {
                cap *= 2;
                buf = (char *)realloc(buf, cap * (size_t)L);
            }
            memcpy(buf + cnt * (size_t)L, d, (size_t)L);
            cnt++;
        } while (next_perm(d, L));
    }
    g_L = L;
    qsort(buf, cnt, (size_t)L, cmp_str);
    for (size_t i = 0; i < cnt; i++) {
        seq_t v = 0;
        for (int j = 0; j < L; j++) v = v * 10 + (int)buf[i * (size_t)L + j];
        push_term(v);
    }
    free(buf);
}

seq_t a(idx_t n) {
    while (nterms < n) {
        curL++;
        gen_length(curL);
    }
    return terms[n - 1];
}
