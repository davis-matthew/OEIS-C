/*
 * A089853  Permutation of natural numbers induced by Catalan automorphism
 *          *A089853 acting on the binary trees/parenthesizations encoded by
 *          A014486/A063171.
 * https://oeis.org/A089853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank in A014486 of the Dyck word 1 C 0 1 A 0 B, where the
 *            n-th Dyck word is 1 A 0 (1 B 0 C); the word is left unchanged if
 *            its right part is empty
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A089853_MAXL 72

static uint64_t A089853_ways[A089853_MAXL + 2][A089853_MAXL + 2];
static uint64_t A089853_cat[40];
static int A089853_init = 0;

static void A089853_setup(void)
{
    int r, b;
    if (A089853_init) return;
    for (r = 0; r <= A089853_MAXL; r++)
        for (b = 0; b <= A089853_MAXL; b++)
            A089853_ways[r][b] = 0;
    A089853_ways[0][0] = 1;
    for (r = 1; r <= A089853_MAXL; r++) {
        for (b = 0; b <= A089853_MAXL; b++) {
            uint64_t v = 0;
            if (b + 1 <= A089853_MAXL) v += A089853_ways[r - 1][b + 1];
            if (b > 0) v += A089853_ways[r - 1][b - 1];
            A089853_ways[r][b] = v;
        }
    }
    A089853_cat[0] = 1;
    for (r = 0; r < 35; r++)
        A089853_cat[r + 1] = A089853_cat[r] * (uint64_t)(2 * (2 * r + 1)) / (uint64_t)(r + 2);
    A089853_init = 1;
}

/* index of matching 0 for the 1 at position s */
static int A089853_match(const char *w, int s)
{
    int d = 0, i = s;
    for (;; i++) {
        if (w[i]) d++; else d--;
        if (d == 0) return i;
    }
}

seq_t a(idx_t n)
{
    char w[A089853_MAXL + 2], o[A089853_MAXL + 2];
    int k, len, pos, bal, m, p;
    uint64_t m_idx, cum = 0, idx;

    if (n == 0) return (seq_t)0;
    A089853_setup();
    m_idx = (uint64_t)n - 1;
    k = 1;
    while (cum + A089853_cat[k] <= m_idx) {
        cum += A089853_cat[k];
        k++;
    }
    idx = m_idx - cum;
    len = 2 * k;

    /* unrank */
    bal = 0;
    for (pos = 0; pos < len; pos++) {
        int rem = len - pos;
        uint64_t c0 = 0;
        if (bal > 0) c0 = A089853_ways[rem - 1][bal - 1];
        if (idx < c0) { w[pos] = 0; bal--; }
        else { idx -= c0; w[pos] = 1; bal++; }
    }

    /* transform: w = 1 X 0 Y, Y = 1 B 0 C  ->  1 C 0 1 X 0 B */
    m = A089853_match(w, 0);
    if (m == len - 1) {
        for (p = 0; p < len; p++) o[p] = w[p];
    } else {
        int ys = m + 1;
        int bm = A089853_match(w, ys);
        int q = 0, i;
        o[q++] = 1;
        for (i = bm + 1; i < len; i++) o[q++] = w[i];      /* C */
        o[q++] = 0;
        o[q++] = 1;
        for (i = 1; i < m; i++) o[q++] = w[i];             /* X */
        o[q++] = 0;
        for (i = ys + 1; i < bm; i++) o[q++] = w[i];       /* B */
    }

    /* rank */
    {
        uint64_t r = 0;
        uint64_t base = 0;
        int j;
        for (j = 1; j < k; j++) base += A089853_cat[j];
        bal = 0;
        for (pos = 0; pos < len; pos++) {
            int rem = len - pos;
            if (o[pos]) {
                if (bal > 0) r += A089853_ways[rem - 1][bal - 1];
                bal++;
            } else {
                bal--;
            }
        }
        return (seq_t)(base + r + 1);
    }
}
