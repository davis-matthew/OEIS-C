/*
 * A005535  Number of semi-regular digraphs (with loops) on n unlabeled nodes
 *          with each node having out-degree 3.
 * https://oeis.org/A005535
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Burnside's lemma over cycle types lambda of S_n: a(n) =
 *            Sum_{lambda |- n} (1/z(lambda)) * Prod_{cycles of length k in
 *            lambda} f(k), where f(k) = C(c1,3) + c1*c2 + c3 and c1, c2, c3
 *            are the numbers of 1-, 2- and 3-cycles of sigma^k. That is the
 *            number of 3-subsets fixed by sigma^k. The sum is take...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   is_prime, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_is_fixed_width.h"

static int g_n, g_np, g_d;
static uint64_t *g_pr, *g_invk, *g_invf, *g_acc;
static int g_part[200], g_mult[200];
static uint64_t g_f[200];

static uint64_t pmod(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void leaf(void)
{
    int i, j, pi;
    for (i = 0; i < g_d; i++) {
        uint64_t k = (uint64_t)g_part[i], c1 = 0, c2 = 0, c3 = 0;
        for (j = 0; j < g_d; j++) {
            uint64_t L = (uint64_t)g_part[j], g = ugcd(L, k), len = L / g;
            if (len == 1) c1 += g * (uint64_t)g_mult[j];
            else if (len == 2) c2 += g * (uint64_t)g_mult[j];
            else if (len == 3) c3 += g * (uint64_t)g_mult[j];
        }
        g_f[i] = (c1 >= 3 ? c1 * (c1 - 1) * (c1 - 2) / 6 : 0) + c1 * c2 + c3;
        if (g_f[i] == 0) return;
    }
    for (pi = 0; pi < g_np; pi++) {
        uint64_t p = g_pr[pi], t = 1;
        uint64_t *ik = g_invk + (size_t)pi * (g_n + 1);
        uint64_t *ifc = g_invf + (size_t)pi * (g_n + 1);
        for (i = 0; i < g_d; i++) {
            uint64_t b = (g_f[i] % p) * ik[g_part[i]] % p;
            t = t * pmod(b, (uint64_t)g_mult[i], p) % p;
            t = t * ifc[g_mult[i]] % p;
        }
        g_acc[pi] += t;
        if (g_acc[pi] >= p) g_acc[pi] -= p;
    }
}

static void rec(int rem, int maxpart)
{
    int k, m;
    if (rem == 0) { leaf(); return; }
    for (k = (rem < maxpart ? rem : maxpart); k >= 1; k--) {
        for (m = 1; m * k <= rem; m++) {
            g_part[g_d] = k;
            g_mult[g_d] = m;
            g_d++;
            rec(rem - k * m, k - 1);
            g_d--;
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t c3n, cand;
    int bits = 0, P, i, j, k;
    seq_t res = 0;

    g_n = (int)n;
    c3n = (uint64_t)n * (n - 1) * (n - 2) / 6;
    while (c3n) { bits++; c3n >>= 1; }
    P = (int)(((uint64_t)n * (uint64_t)bits + 30) / 31) + 1;
    if (seq_is_fixed_width() && P > 5) P = 5;
    g_np = P;

    g_pr = (uint64_t *)malloc(sizeof(uint64_t) * P);
    g_acc = (uint64_t *)calloc((size_t)P, sizeof(uint64_t));
    g_invk = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)P * (n + 1));
    g_invf = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)P * (n + 1));

    cand = 4294967295ULL;
    for (i = 0; i < P; i++) {
        while (!is_prime(cand)) cand--;
        g_pr[i] = cand--;
    }
    for (i = 0; i < P; i++) {
        uint64_t p = g_pr[i], fct = 1;
        uint64_t *ik = g_invk + (size_t)i * (n + 1);
        uint64_t *ifc = g_invf + (size_t)i * (n + 1);
        ik[0] = 0;
        ifc[0] = 1;
        for (k = 1; k <= (int)n; k++) {
            ik[k] = pmod((uint64_t)k, p - 2, p);
            fct = fct * (uint64_t)k % p;
            ifc[k] = pmod(fct, p - 2, p);
        }
    }

    g_d = 0;
    rec((int)n, (int)n);

    /* Garner mixed-radix reconstruction */
    {
        uint64_t *v = (uint64_t *)malloc(sizeof(uint64_t) * P);
        for (i = 0; i < P; i++) {
            uint64_t p = g_pr[i], x = 0, prod = 1;
            for (j = 0; j < i; j++) {
                x = (x + (v[j] % p) * prod) % p;
                prod = prod * (g_pr[j] % p) % p;
            }
            x = (g_acc[i] + p - x) % p;
            v[i] = x * pmod(prod, p - 2, p) % p;
        }
        for (i = P - 1; i >= 0; i--) {
            res = res * (seq_t)g_pr[i] + (seq_t)v[i];
        }
        free(v);
    }

    free(g_pr);
    free(g_acc);
    free(g_invk);
    free(g_invf);
    return res;
}
