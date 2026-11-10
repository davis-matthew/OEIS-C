/*
 * A075644  Final terms of groups in A075643.
 * https://oeis.org/A075644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = last term of group n, built by greedy simulation: group n
 *            has terms g_1..g_n with g_k a multiple of k, g_1 = least unused
 *            number, g_k = least unused multiple of k for k < n, and g_n =
 *            least unused multiple of n making the group sum divisible by n+1
 *            (see A075643)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *g_used = NULL;
static uint64_t g_cap = 0;
static uint64_t *g_ptr = NULL;
static uint64_t *g_res = NULL;
static uint64_t g_groups = 0;
static uint64_t g_ptrcap = 0;

static void A075644_ensure(uint64_t v)
{
    if (v < g_cap) return;
    uint64_t nc = g_cap ? g_cap : 1024;
    while (nc <= v) nc <<= 1;
    unsigned char *p = (unsigned char *)realloc(g_used, (size_t)nc);
    if (!p) abort();
    memset(p + g_cap, 0, (size_t)(nc - g_cap));
    g_used = p;
    g_cap = nc;
}

static void A075644_ensure_groups(uint64_t n)
{
    if (n < g_ptrcap) return;
    uint64_t nc = g_ptrcap ? g_ptrcap : 64;
    while (nc <= n) nc <<= 1;
    g_ptr = (uint64_t *)realloc(g_ptr, (size_t)(nc * sizeof(uint64_t)));
    g_res = (uint64_t *)realloc(g_res, (size_t)(nc * sizeof(uint64_t)));
    if (!g_ptr || !g_res) abort();
    g_ptrcap = nc;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    A075644_ensure_groups(N + 1);
    while (g_groups < N) {
        uint64_t m = g_groups + 1;
        uint64_t sum = 0;
        uint64_t k;
        g_ptr[m] = m;
        for (k = 1; k < m; k++) {
            uint64_t v = g_ptr[k];
            A075644_ensure(v);
            while (g_used[v]) {
                v += k;
                A075644_ensure(v);
            }
            g_used[v] = 1;
            g_ptr[k] = v + k;
            sum += v;
        }
        {
            uint64_t v = g_ptr[m];
            A075644_ensure(v);
            while (1) {
                A075644_ensure(v);
                if (!g_used[v] && (sum + v) % (m + 1) == 0) break;
                v += m;
            }
            g_used[v] = 1;
            g_res[m] = v;
            /* advance pointer over used entries */
            {
                uint64_t p = g_ptr[m];
                A075644_ensure(p);
                while (g_used[p]) {
                    p += m;
                    A075644_ensure(p);
                }
                g_ptr[m] = p;
            }
        }
        g_groups = m;
    }
    return (seq_t)g_res[N];
}
