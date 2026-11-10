/*
 * A091410  Values of n such that len_x(n) = 0 in A090822.
 * https://oeis.org/A091410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th index m such that the prefix a(1..m-1) of Gijswijt's
 *            sequence A090822 is y^k (x empty) with k = A090822(m); computed
 *            by generating A090822 and testing each index (a(1)=1, a(2)=2,
 *            a(3)=3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static unsigned char *A091410_g = NULL;   /* Gijswijt terms, 0-based */
static uint32_t *A091410_match = NULL;    /* match[b]: length of suffix with period b */
static idx_t A091410_len = 0;             /* terms generated */
static idx_t A091410_cap = 0;
static idx_t *A091410_hits = NULL;
static idx_t A091410_nh = 0, A091410_hcap = 0;

static void A091410_add_hit(idx_t n)
{
    if (A091410_nh >= A091410_hcap) {
        idx_t nc = A091410_hcap ? A091410_hcap * 2 : 64;
        A091410_hits = (idx_t *)realloc(A091410_hits, nc * sizeof(idx_t));
        A091410_hcap = nc;
    }
    A091410_hits[A091410_nh++] = n;
}

static void A091410_ensure(idx_t need)
{
    if (A091410_cap >= need) return;
    idx_t nc = A091410_cap ? A091410_cap : 1024;
    while (nc < need) nc *= 2;
    A091410_g = (unsigned char *)realloc(A091410_g, nc);
    A091410_match = (uint32_t *)realloc(A091410_match, (nc + 1) * sizeof(uint32_t));
    for (idx_t i = A091410_cap; i <= nc; ++i) A091410_match[i] = 0;
    A091410_cap = nc;
}

static void A091410_step(void)
{
    idx_t L = A091410_len;
    A091410_ensure(L + 2);
    unsigned char *g = A091410_g;
    uint32_t *match = A091410_match;
    uint32_t best = 1;
    for (idx_t b = 1; b <= L; ++b) {
        uint32_t c = match[b] / (uint32_t)b + 1;
        if (c > best) best = c;
    }
    idx_t n = L + 1;
    if (n == 2) {
        A091410_add_hit(n);
    } else if (best >= 2 && (L % best) == 0) {
        idx_t b = L / best;
        if ((idx_t)match[b] == L - b) A091410_add_hit(n);
    }
    unsigned char t = (unsigned char)best;
    g[L] = t;
    for (idx_t b = 1; b <= L; ++b) {
        match[b] = (g[L - b] == t) ? match[b] + 1 : 0;
    }
    A091410_len = L + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A091410_len == 0) {
        A091410_ensure(8);
        A091410_g[0] = 1;
        A091410_len = 1;
        A091410_add_hit(1);
    }
    while (A091410_nh < n) A091410_step();
    return (seq_t)A091410_hits[n - 1];
}
