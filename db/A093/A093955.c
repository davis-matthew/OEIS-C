/*
 * A093955  a(n) = A090822(n) - 1.
 * https://oeis.org/A093955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A090822(n) - 1, where A090822 is Gijswijt's sequence
 *            (a(1)=1; a(n) = max k such that a(1..n-1) ends in k repeated
 *            blocks); computed by direct search over block lengths
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 8833 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *G_seq = NULL;
static idx_t G_len = 0;
static idx_t G_cap = 0;

static void G_push(seq_t v)
{
    if (G_len >= G_cap) {
        idx_t nc = G_cap ? G_cap * 2 : 64;
        G_seq = mem_resize(seq_t, G_seq, nc);
        G_cap = nc;
    }
    G_seq[G_len++] = v;
}

static void G_next(void)
{
    idx_t len = G_len;
    idx_t best = 1;
    for (idx_t L = 1; L <= len; ++L) {
        /* count matching positions backward: s[i] == s[i-L] */
        idx_t r = 0;
        idx_t i = len;            /* i-1 is the current index */
        while (i > L && G_seq[i - 1] == G_seq[i - 1 - L]) {
            ++r;
            --i;
        }
        idx_t k = r / L + 1;
        if (k > best) best = k;
    }
    G_push((seq_t)best);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (G_len == 0) {
        G_push((seq_t)1);
        G_push((seq_t)1);
    }
    while (G_len < n) G_next();
    return G_seq[n - 1] - 1;
}
