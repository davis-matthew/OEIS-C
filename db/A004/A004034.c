/*
 * A004034  Theta series of Leech lattice with respect to deep hole of type
 *          A_1.
 * https://oeis.org/A004034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n+2)] X^2*Y^2*(3*X^2 + 10*X*Y + 3*Y^2)/16, where X =
 *            theta_2(q)^4, Y = theta_3(q)^4. Equivalently, with U =
 *            theta_2^4*theta_3^4/16 = Sum_{k>=1} g(k) q^k, g(k) = Sum_{d|k,
 *            k/d odd} d^3, a(n) = 16 * [q^(n+2)] U^2 * (768*Sum_k g(k) q^(2k)
 *            + 160*U + 3*theta_3(q)^8), theta_3^8 = 1 + Sum_{m>...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

static uint64_t *g_tab = NULL;           /* g(k) */
static uint64_t *r8_tab = NULL;          /* r8(m), r8(0) = 1 */
static unsigned __int128 *u2_tab = NULL; /* coefficients of U^2 */
static idx_t g_cap = 0;

static void ensure(idx_t m)
{
    idx_t newcap, i, d, e, k;
    int64_t *s8;
    if (m < g_cap)
        return;
    newcap = m + m / 4 + 16;
    g_tab = (uint64_t *)realloc(g_tab, newcap * sizeof(uint64_t));
    r8_tab = (uint64_t *)realloc(r8_tab, newcap * sizeof(uint64_t));
    u2_tab = (unsigned __int128 *)realloc(u2_tab, newcap * sizeof(unsigned __int128));
    s8 = (int64_t *)calloc(newcap, sizeof(int64_t));
    for (i = 0; i < newcap; i++)
        g_tab[i] = 0;
    for (e = 1; e < newcap; e += 2)
        for (d = 1; d * e < newcap; d++)
            g_tab[d * e] += (uint64_t)d * d * d;
    for (d = 1; d < newcap; d++) {
        int64_t c = (int64_t)(d * d * d);
        for (k = d; k < newcap; k += d) {
            if (((k + d) & 1) == 0)
                s8[k] += c;
            else
                s8[k] -= c;
        }
    }
    r8_tab[0] = 1;
    for (k = 1; k < newcap; k++)
        r8_tab[k] = (uint64_t)(16 * s8[k]);
    free(s8);
    for (i = g_cap; i < newcap; i++) {
        unsigned __int128 s = 0;
        idx_t a1;
        for (a1 = 1; 2 * a1 < i; a1++)
            s += (unsigned __int128)g_tab[a1] * g_tab[i - a1];
        s *= 2;
        if (i >= 2 && (i & 1) == 0)
            s += (unsigned __int128)g_tab[i / 2] * g_tab[i / 2];
        u2_tab[i] = s;
    }
    g_cap = newcap;
}

seq_t a(idx_t n)
{
    idx_t N = n + 2, i;
    seq_t s = 0;
    ensure(N + 1);
    for (i = 2; i <= N; i++) {
        idx_t j = N - i;
        unsigned __int128 t = 0;
        if (j >= 2 && (j & 1) == 0)
            t += (unsigned __int128)768 * g_tab[j / 2];
        t += (unsigned __int128)160 * g_tab[j];
        t += (unsigned __int128)3 * r8_tab[j];
        s += u128_to_seq(u2_tab[i]) * u128_to_seq(t);
    }
    return s * 16;
}
