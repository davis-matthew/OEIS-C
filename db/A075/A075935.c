/*
 * A075935  Basis for code in A075934.
 * https://oeis.org/A075935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(2^n), b = A075934 (binary lexicode, d=6); the lexicode
 *            is linear, so a(n) is the smallest word at distance >= 6 from
 *            the span of a(0..n-1). Found by syndrome search: smallest-length
 *            L, bad syndromes = XORs of <= 5 columns, first good coset leader
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

#define A075935_MAXL 512

static unsigned long long A075935_col[A075935_MAXL];
static unsigned char *A075935_bad;
static int A075935_L;

static void A075935_rec(int start, int depth, unsigned long long s)
{
    A075935_bad[s] = 1;
    if (depth < 5) {
        for (int i = start; i < A075935_L; ++i)
            A075935_rec(i + 1, depth + 1, s ^ A075935_col[i]);
    }
}

static int A075935_highbit(seq_t v)
{
    int p = -1;
    while (v != 0) { ++p; v >>= 1; }
    return p;
}

seq_t a(idx_t n)
{
    static seq_t *res = NULL;
    static size_t cnt = 0, cap = 0;

    if (!res) {
        cap = 64;
        res = mem_new(seq_t, cap);
        if (!res) return (seq_t)0;
        cnt = 0;
    }

    while (cnt <= (size_t)n) {
        size_t m = cnt;
        /* build fully reduced echelon basis */
        seq_t *ech = mem_new(seq_t, m + 1);
        int *piv = (int *)malloc((m + 1) * sizeof(int));
        size_t ne = 0;
        for (size_t i = 0; i < m; ++i) {
            seq_t v = res[i];
            for (size_t j = 0; j < ne; ++j)
                if (((v >> piv[j]) & 1) != 0) v ^= ech[j];
            int p = A075935_highbit(v);
            for (size_t j = 0; j < ne; ++j)
                if (((ech[j] >> p) & 1) != 0) ech[j] ^= v;
            ech[ne] = v;
            piv[ne] = p;
            ++ne;
        }
        int L = (m == 0) ? 1 : A075935_highbit(res[m - 1]) + 1;
        seq_t found = 0;
        bool ok = false;
        for (; L < A075935_MAXL && !ok; ++L) {
            int np[A075935_MAXL];
            int q = 0;
            for (int i = 0; i < L; ++i) {
                bool isp = false;
                for (size_t j = 0; j < ne; ++j) if (piv[j] == i) { isp = true; break; }
                if (!isp) np[q++] = i;
            }
            if (q == 0) continue;
            if (q > 28) break;
            for (int i = 0; i < L; ++i) {
                seq_t v = (seq_t)1 << i;
                for (size_t j = 0; j < ne; ++j)
                    if (((v >> piv[j]) & 1) != 0) v ^= ech[j];
                unsigned long long c = 0;
                for (int t = 0; t < q; ++t)
                    if (((v >> np[t]) & 1) != 0) c |= (1ULL << t);
                A075935_col[i] = c;
            }
            size_t sz = (size_t)1 << q;
            A075935_bad = (unsigned char *)calloc(sz, 1);
            if (!A075935_bad) break;
            A075935_L = L;
            A075935_rec(0, 0, 0ULL);
            for (size_t c = 0; c < sz; ++c) {
                if (!A075935_bad[c]) {
                    seq_t x = 0;
                    for (int t = 0; t < q; ++t)
                        if (((c >> t) & 1) != 0) x |= (seq_t)1 << np[t];
                    found = x;
                    ok = true;
                    break;
                }
            }
            free(A075935_bad);
            A075935_bad = NULL;
        }
        mem_free(ech);
        free(piv);
        if (!ok) return (seq_t)0;
        if (cnt == cap) {
            cap *= 2;
            seq_t *tmp = mem_resize(seq_t, res, cap);
            if (!tmp) return (seq_t)0;
            res = tmp;
        }
        res[cnt++] = found;
    }
    return res[n];
}
