/*
 * A202014  Smallest m such that A063882(m) = n.
 * https://oeis.org/A202014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A063882(m) = n, where A063882(m) =
 *            A063882(m - A063882(m-1)) + A063882(m - A063882(m-4)),
 *            A063882(1..4) = 1; computed by generating A063882 and recording
 *            first occurrences
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9942 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t *s063882 = NULL;  /* sequence values, 1-based */
static idx_t *first063882 = NULL;  /* first index with given value, 0 = none yet */
static size_t cap063882 = 0;
static size_t len063882 = 0;

static void A202014_grow(void)
{
    size_t nc = cap063882 ? cap063882 * 2 : 64;
    idx_t *ns = (idx_t *)realloc(s063882, nc * sizeof(idx_t));
    if (!ns) abort();
    s063882 = ns;
    idx_t *nf = (idx_t *)realloc(first063882, nc * sizeof(idx_t));
    if (!nf) abort();
    first063882 = nf;
    for (size_t i = cap063882; i < nc; ++i) { s063882[i] = 0; first063882[i] = 0; }
    cap063882 = nc;
}

seq_t a(idx_t n)
{
    if (len063882 == 0) {
        A202014_grow();
        for (size_t i = 1; i <= 4; ++i) s063882[i] = 1;
        first063882[1] = 1;
        len063882 = 4;
    }
    for (;;) {
        if (n < (idx_t)cap063882 && first063882[n] != 0)
            return (seq_t)first063882[n];
        size_t i = len063882 + 1;
        if (i >= cap063882) A202014_grow();
        idx_t v = s063882[i - s063882[i - 1]] + s063882[i - s063882[i - 4]];
        s063882[i] = v;
        if (first063882[v] == 0) first063882[v] = (idx_t)i;
        len063882 = i;
    }
}
