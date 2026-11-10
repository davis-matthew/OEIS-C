/*
 * A225589  Inverse of permutation in A217122.
 * https://oeis.org/A225589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A217122 (inverse permutation); computed
 *            by simulating A217122 until value n appears
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9517 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static idx_t *inv_tab = NULL;   /* inv_tab[v] = index n with A217122(n) = v, 0 if not yet seen */
static size_t inv_cap = 0;
static idx_t inv_cnt = 0;       /* number of terms of A217122 generated */
static size_t inv_lo = 1;       /* smallest value not yet used */

static void inv_ensure(size_t need)
{
    if (need < inv_cap) return;
    size_t nc = inv_cap ? inv_cap : 1024;
    while (nc <= need) nc <<= 1;
    idx_t *p = (idx_t*)realloc(inv_tab, nc * sizeof(idx_t));
    if (!p) abort();
    for (size_t i = inv_cap; i < nc; ++i) p[i] = 0;
    inv_tab = p;
    inv_cap = nc;
}

seq_t a(idx_t n)
{
    size_t v = (size_t)(long long)n;
    inv_ensure(v + 1);
    while (inv_tab[v] == 0) {
        idx_t m = ++inv_cnt;
        unsigned k = (unsigned)__builtin_popcountll((unsigned long long)m);
        size_t pos = inv_lo;
        unsigned cnt = 0;
        for (;;) {
            inv_ensure(pos + 1);
            if (inv_tab[pos] == 0) {
                if (++cnt == k) break;
            }
            ++pos;
        }
        inv_tab[pos] = m;
        while (1) {
            inv_ensure(inv_lo + 1);
            if (inv_tab[inv_lo] == 0) break;
            ++inv_lo;
        }
    }
    return (seq_t)inv_tab[v];
}
