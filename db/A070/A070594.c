/*
 * A070594  Numbers in A070938 that set a new record for digital sums and
 *          ending digits.
 * https://oeis.org/A070594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest x = 10^k*h + s (s = digit sum of x, k = digits
 *            of s, s | x) for the n-th record s; computed by BFS search for
 *            the smallest such x per digit sum s
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define A070594_SMAX 6000

static unsigned char *A070594_md[A070594_SMAX];
static int A070594_ml[A070594_SMAX];
static char A070594_done[A070594_SMAX];

static int A070594_gcd(int a, int b)
{
    while (b) { int t = a % b; a = b; b = t; }
    return a;
}

/* smallest x with digit sum s, ending in decimal digits of s, divisible by s */
static void A070594_compute(int s)
{
    if (A070594_done[s]) return;
    A070594_done[s] = 1;
    A070594_ml[s] = -1;
    A070594_md[s] = NULL;

    int sd[8], k = 0, tmp = s, ds = 0, p10 = 1;
    while (tmp) { sd[k++] = tmp % 10; ds += tmp % 10; tmp /= 10; p10 *= 10; }

    if (s < 10) {
        A070594_md[s] = (unsigned char *)malloc(1);
        A070594_md[s][0] = (unsigned char)s;
        A070594_ml[s] = 1;
        return;
    }

    int t = s - ds;
    int g = s / A070594_gcd(s, p10);
    int W = t + 1;
    size_t ns = (size_t)g * (size_t)W;
    int *par = (int *)malloc(ns * sizeof(int));
    unsigned char *dg = (unsigned char *)malloc(ns);
    unsigned char *vis = (unsigned char *)calloc(ns, 1);
    int *q = (int *)malloc(ns * sizeof(int));
    size_t qh = 0, qt = 0;
    int target = 0 * W + t;
    bool found = false;

    vis[0] = 1;
    par[0] = -1;
    q[qt++] = 0;
    while (qh < qt && !found) {
        int st = q[qh++];
        int r = st / W, sm = st % W;
        int d0 = (st == 0) ? 1 : 0;
        for (int d = d0; d <= 9; d++) {
            if (sm + d > t) break;
            int nr = (r * 10 + d) % g;
            int nst = nr * W + sm + d;
            if (!vis[nst]) {
                vis[nst] = 1;
                par[nst] = st;
                dg[nst] = (unsigned char)d;
                q[qt++] = nst;
                if (nst == target) { found = true; break; }
            }
        }
    }
    if (found) {
        int hl = 0;
        for (int st = target; st != 0; st = par[st]) hl++;
        int L = hl + k;
        unsigned char *res = (unsigned char *)malloc((size_t)L);
        int pos = hl - 1;
        for (int st = target; st != 0; st = par[st]) res[pos--] = dg[st];
        for (int i = 0; i < k; i++) res[hl + i] = (unsigned char)sd[k - 1 - i];
        A070594_md[s] = res;
        A070594_ml[s] = L;
    }
    free(par); free(dg); free(vis); free(q);
}

/* is m(a) < m(b) ? infinite never less */
static bool A070594_less(int a, int b)
{
    if (A070594_ml[a] < 0) return false;
    if (A070594_ml[b] < 0) return true;
    if (A070594_ml[a] != A070594_ml[b]) return A070594_ml[a] < A070594_ml[b];
    return memcmp(A070594_md[a], A070594_md[b], (size_t)A070594_ml[a]) < 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (int s = 1; s < A070594_SMAX; s++) {
        A070594_compute(s);
        if (A070594_ml[s] < 0) continue;
        int L = A070594_ml[s];
        bool rec = true;
        for (int s2 = s + 1; s2 <= 9 * L && s2 < A070594_SMAX; s2++) {
            A070594_compute(s2);
            if (!A070594_less(s, s2)) { rec = false; break; }
        }
        if (rec) {
            count++;
            if (count == n) {
                seq_t v = 0;
                for (int i = 0; i < L; i++) {
                    v = v * 10;
                    v = v + (seq_t)A070594_md[s][i];
                }
                return v;
            }
        }
    }
    return 0;
}
