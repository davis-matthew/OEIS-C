/*
 * A000294  Expansion of g.f. Product_{k >= 1} (1 - x^k)^(-k*(k+1)/2).
 * https://oeis.org/A000294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n)*Sum_{k=1..n} b(k)*a(n-k), a(0) = 1, where b(k) =
 *            Sum_{d|k} d*d*(d+1)/2 = (sigma_2(k)+sigma_3(k))/2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 32, u64 n <= 75, u128 n <= 183,
 *            big any n
 * Helpers:   seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_is_big.h"

static uint64_t *Bt = NULL;
static idx_t Bn = 0;

static void need_b(idx_t N)
{
    idx_t m, d, k;
    if (N < Bn) return;
    m = N + 1;
    if (m < 2 * Bn) m = 2 * Bn;
    free(Bt);
    Bt = (uint64_t *)calloc((size_t)m, sizeof(uint64_t));
    for (d = 1; d < m; d++) {
        uint64_t t = (uint64_t)d * d * (d + 1) / 2;
        for (k = d; k < m; k += d) Bt[k] += t;
    }
    Bn = m;
}

/* fixed-width path: exact values kept in unsigned __int128, sums in 192 bits */
static unsigned __int128 *A128 = NULL;
static idx_t A128n = 0, A128cap = 0;

static unsigned __int128 fixed_term(idx_t n)
{
    const unsigned __int128 M64 = (((unsigned __int128)1) << 64) - 1;
    idx_t m, k;
    if (n >= A128cap) {
        idx_t nc = n + 1;
        if (nc < 2 * A128cap) nc = 2 * A128cap;
        A128 = (unsigned __int128 *)realloc(A128, (size_t)nc * sizeof(unsigned __int128));
        A128cap = nc;
    }
    need_b(n);
    for (m = A128n; m <= n; m++) {
        unsigned __int128 lo = 0, hi = 0, t, r, q1, q0;
        if (m == 0) { A128[0] = 1; continue; }
        for (k = 1; k <= m; k++) {
            unsigned __int128 v = A128[m - k];
            uint64_t b = Bt[k];
            unsigned __int128 p0 = (unsigned __int128)b * (uint64_t)(v & M64);
            unsigned __int128 p1 = (unsigned __int128)b * (uint64_t)(v >> 64);
            unsigned __int128 old = lo;
            lo += p0;
            if (lo < old) hi++;
            old = lo;
            lo += (p1 & M64) << 64;
            if (lo < old) hi++;
            hi += p1 >> 64;
        }
        r = hi % m;
        t = (r << 64) | (lo >> 64);
        q1 = t / m;
        r = t % m;
        t = (r << 64) | (lo & M64);
        q0 = t / m;
        A128[m] = (q1 << 64) + q0;
    }
    if (n + 1 > A128n) A128n = n + 1;
    return A128[n];
}

/* arbitrary-precision path */
static seq_t *AB = NULL;
static idx_t ABn = 0, ABcap = 0;

static seq_t big_term(idx_t n)
{
    idx_t m, k;
    if (n >= ABcap) {
        idx_t nc = n + 1, i;
        seq_t *nw;
        if (nc < 2 * ABcap) nc = 2 * ABcap;
        nw = (seq_t *)calloc((size_t)nc, sizeof(seq_t));
        for (i = 0; i < ABn; i++) nw[i] = AB[i];
        free(AB);
        AB = nw;
        ABcap = nc;
    }
    need_b(n);
    for (m = ABn; m <= n; m++) {
        seq_t s = 0;
        if (m == 0) { AB[0] = 1; continue; }
        for (k = 1; k <= m; k++) s += (seq_t)Bt[k] * AB[m - k];
        AB[m] = s / m;
    }
    if (n + 1 > ABn) ABn = n + 1;
    return AB[n];
}

seq_t a(idx_t n)
{
    static int mode = -1;
    if (mode < 0) mode = seq_is_big();
    if (mode) return big_term(n);
    {
        unsigned __int128 v = fixed_term(n);
        seq_t c = (seq_t)4294967296ULL;
        seq_t r = (seq_t)(uint64_t)(v >> 64);
        r = r * c * c + (seq_t)(uint64_t)v;
        return r;
    }
}
