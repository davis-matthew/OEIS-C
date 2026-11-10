/*
 * A063175  Composites for which the row of the prime-composite array
 *          (A063173) includes the leftmost element of a zero-only diagonal
 *          (A067677).
 * https://oeis.org/A063175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(A067677(n)), where c(m) is the m-th composite. Computed
 *            by a search: m is in A067677 if for all k>=1, prime p_k does not
 *            divide c(m+k-1) (stop once c(m+k-1) < p_k), using a growing
 *            sieve
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *g_comp = NULL;   /* 1-based composites */
static uint32_t *g_prm = NULL;    /* 1-based primes */
static size_t g_ncomp = 0, g_nprm = 0;
static size_t g_limit = 0;
static uint64_t *g_found = NULL;  /* 1-based list of m values */
static size_t g_nfound = 0, g_foundcap = 0;
static uint64_t g_next = 1;

static void a063175_build(size_t limit)
{
    char *s = (char *)malloc(limit + 1);
    size_t i, j;
    if (!s) abort();
    for (i = 0; i <= limit; i++) s[i] = 1;
    s[0] = 0; s[1] = 0;
    for (i = 2; i * i <= limit; i++)
        if (s[i])
            for (j = i * i; j <= limit; j += i) s[j] = 0;
    free(g_comp); free(g_prm);
    g_comp = (uint32_t *)malloc((limit + 2) * sizeof(uint32_t));
    g_prm = (uint32_t *)malloc((limit + 2) * sizeof(uint32_t));
    if (!g_comp || !g_prm) abort();
    g_ncomp = 0; g_nprm = 0;
    for (i = 2; i <= limit; i++) {
        if (s[i]) g_prm[++g_nprm] = (uint32_t)i;
        else g_comp[++g_ncomp] = (uint32_t)i;
    }
    free(s);
    g_limit = limit;
}

/* 1 = zero-only diagonal, 0 = not, -1 = need larger sieve */
static int a063175_check(uint64_t m)
{
    uint64_t k;
    for (k = 1;; k++) {
        uint64_t ci = m + k - 1;
        uint32_t c, p;
        if (ci > g_ncomp || k > g_nprm) return -1;
        c = g_comp[ci];
        p = g_prm[k];
        if (c < p) return 1;
        if (c % p == 0) return 0;
    }
}

seq_t a(idx_t n)
{
    if (g_limit == 0) a063175_build(1u << 16);
    while (g_nfound < (size_t)n) {
        int r = a063175_check(g_next);
        if (r < 0) {
            a063175_build(g_limit * 2);
            continue;
        }
        if (r == 1) {
            if (g_nfound + 2 > g_foundcap) {
                g_foundcap = g_foundcap ? g_foundcap * 2 : 64;
                g_found = (uint64_t *)realloc(g_found, g_foundcap * sizeof(uint64_t));
                if (!g_found) abort();
            }
            g_found[++g_nfound] = g_next;
        }
        g_next++;
    }
    return (seq_t)g_comp[g_found[n]];
}
