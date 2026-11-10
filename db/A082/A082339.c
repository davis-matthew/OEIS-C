/*
 * A082339  Permutation of natural numbers induced by the Catalan bijection
 *          gma082339 acting on the parenthesizations encoded by
 *          A014486/A063171.
 * https://oeis.org/A082339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A082358... no closed form; a(n) = rank of gma082339(w)
 *            where w is the n-th totally balanced binary word (A014486) and
 *            gma082339(() . T) = (() . gma082339(T)), gma082339(((L1 . L2) .
 *            R)) = ((L1 . R) . gma082339(L2)), gma082339(nil) = nil; result
 *            re-ranked within A014486
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t gma_dp[72][72];
static bool gma_dp_ready = false;

static void gma_init(void)
{
    if (gma_dp_ready) return;
    for (int r = 0; r < 72; r++)
        for (int b = 0; b < 72; b++)
            gma_dp[r][b] = 0;
    gma_dp[0][0] = 1;
    for (int r = 1; r < 72; r++)
        for (int b = 0; b < 70; b++) {
            uint64_t v = 0;
            if (b > 0) v += gma_dp[r - 1][b - 1];
            v += gma_dp[r - 1][b + 1];
            gma_dp[r][b] = v;
        }
    gma_dp_ready = true;
}

/* index of matching ')' for '(' at position p */
static int gma_match(const unsigned char *s, int p)
{
    int d = 0;
    for (int i = p;; i++) {
        d += s[i] ? 1 : -1;
        if (d == 0) return i;
    }
}

/* apply gma082339 to the list encoded in s[0..len) */
static void gma_apply(const unsigned char *s, int len, unsigned char *out)
{
    int lo = 0, hi = len, o = 0;
    while (lo < hi) {
        if (s[lo + 1] == 0) {            /* car is empty: (() . T) */
            out[o++] = 1; out[o++] = 0;
            lo += 2;
            continue;
        }
        int m = gma_match(s, lo);        /* end of first element */
        int m1 = gma_match(s, lo + 1);   /* end of first element of car */
        /* L1 = [lo+2, m1), L2 = [m1+1, m), R = [m+1, hi) */
        out[o++] = 1; out[o++] = 1;
        for (int i = lo + 2; i < m1; i++) out[o++] = s[i];
        out[o++] = 0;
        for (int i = m + 1; i < hi; i++) out[o++] = s[i];
        out[o++] = 0;
        /* continue with gma(L2) as the new cdr */
        lo = m1 + 1;
        hi = m;
    }
}

seq_t a(idx_t n)
{
    gma_init();
    if (n == 0) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    int k = 0;
    uint64_t base = 0;
    while (1) {
        uint64_t c = gma_dp[2 * k][0];
        if (m < c) break;
        m -= c;
        base += c;
        k++;
    }
    if (k == 0) return (seq_t)0;
    int len = 2 * k;
    unsigned char w[160], v[160];
    int bal = 0;
    uint64_t idx = m;
    for (int pos = 0; pos < len; pos++) {
        int rem = len - pos;
        uint64_t c0 = 0;
        if (bal > 0) c0 = gma_dp[rem - 1][bal - 1];
        if (idx < c0) { w[pos] = 0; bal--; }
        else { idx -= c0; w[pos] = 1; bal++; }
    }
    gma_apply(w, len, v);
    bal = 0;
    uint64_t r = 0;
    for (int pos = 0; pos < len; pos++) {
        int rem = len - pos;
        uint64_t c0 = 0;
        if (bal > 0) c0 = gma_dp[rem - 1][bal - 1];
        if (v[pos]) { r += c0; bal++; }
        else bal--;
    }
    return (seq_t)base + (seq_t)r;
}
