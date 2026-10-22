/*
 * A002188  Sprague-Grundy value for Grundy's game when starting with n
 *          tokens.
 * https://oeis.org/A002188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, nice
 * Formula:   a(n) = mex{ a(i) XOR a(n-i) : 1 <= i < n/2 }, a(0) = a(1) = a(2)
 *            = 0 (computed iteratively by mex over all unequal splits)
 * Range:     u8 n <= 9904, u16 n <= 9904, u32 n <= 9904, u64 n <= 9904,
 *            u128 n <= 9904, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned *g_cache = NULL;
static idx_t g_len = 0;   /* number of computed entries: indices 0..g_len-1 */

static void a002188_extend(idx_t n) {
    idx_t m, i;
    unsigned char *seen;
    unsigned *nc;
    if (n < g_len) return;
    nc = (unsigned *)realloc(g_cache, (size_t)(n + 1) * sizeof(unsigned));
    if (!nc) return;
    g_cache = nc;
    if (g_len == 0) {
        idx_t k;
        for (k = 0; k <= 2 && k <= n; k++) g_cache[k] = 0;
        g_len = (n < 2) ? n + 1 : 3;
    }
    seen = (unsigned char *)malloc((size_t)(n / 2 + 2));
    if (!seen) return;
    for (m = g_len; m <= n; m++) {
        idx_t cnt = (m - 1) / 2;   /* number of splits i with 1 <= i < m - i */
        unsigned v;
        memset(seen, 0, (size_t)(cnt + 1));
        for (i = 1; i + i < m; i++) {
            unsigned x = g_cache[i] ^ g_cache[m - i];
            if ((idx_t)x <= cnt) seen[x] = 1;
        }
        v = 0;
        while ((idx_t)v <= cnt && seen[v]) v++;
        g_cache[m] = v;
    }
    free(seen);
    g_len = n + 1;
}

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)0;
    a002188_extend(n);
    return (seq_t)g_cache[n];
}
