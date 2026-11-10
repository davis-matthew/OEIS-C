/*
 * A205594  n such that A205592(n) > n.
 * https://oeis.org/A205594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 2 such that A205592(m) > m, where A205592(2) =
 *            1, A205592(3k) = A205592(3k+1) = A205592(2k), A205592(3k+2) =
 *            2*A205592(2k+1); computed by a sieve-style scan
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 967 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *g_vals = NULL;
static idx_t g_len = 0;        /* vals valid for indices 0..g_len-1 */
static idx_t *g_terms = NULL;
static size_t g_nterms = 0, g_capterms = 0;
static idx_t g_scanned = 1;    /* last m scanned */

static void extend_vals(idx_t newlen)
{
    uint64_t *t = (uint64_t *)realloc(g_vals, (size_t)newlen * sizeof(uint64_t));
    if (!t) exit(1);
    g_vals = t;
    const uint64_t CAP = (uint64_t)1 << 62;
    for (idx_t i = g_len; i < newlen; ++i) {
        if (i < 2) { g_vals[i] = 0; continue; }
        if (i == 2) { g_vals[i] = 1; continue; }
        idx_t r = i % 3;
        if (r == 0) {
            g_vals[i] = g_vals[2 * (i / 3)];
        } else if (r == 1) {
            g_vals[i] = g_vals[2 * ((i - 1) / 3)];
        } else {
            uint64_t x = g_vals[2 * ((i - 2) / 3) + 1];
            uint64_t v = x * 2;
            if (v > CAP) v = CAP;
            g_vals[i] = v;
        }
    }
    g_len = newlen;
}

seq_t a(idx_t n)
{
    while (g_nterms < n) {
        idx_t newlen = g_len ? g_len * 2 : 4096;
        extend_vals(newlen);
        for (idx_t m = g_scanned + 1; m < g_len; ++m) {
            if (g_vals[m] > (uint64_t)m) {
                if (g_nterms == g_capterms) {
                    g_capterms = g_capterms ? g_capterms * 2 : 256;
                    idx_t *t = (idx_t *)realloc(g_terms, g_capterms * sizeof(idx_t));
                    if (!t) exit(1);
                    g_terms = t;
                }
                g_terms[g_nterms++] = m;
            }
            g_scanned = m;
        }
    }
    return (seq_t)g_terms[n - 1];
}
