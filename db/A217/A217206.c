/*
 * A217206  Gijswijt's sequence A090822 read mod 2.
 * https://oeis.org/A217206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A090822(n) mod 2, where A090822(1)=1 and, for n>1,
 *            A090822(n) = largest k such that a(1)..a(n-1) = x y^k (computed
 *            directly, memoized)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9515 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *g_seq = NULL;
static idx_t g_len = 0;
static idx_t g_cap = 0;

static void g_ensure(idx_t needed)
{
    if (g_cap >= needed) return;
    idx_t nc = g_cap ? g_cap : 64;
    while (nc < needed) nc <<= 1;
    uint32_t *t = (uint32_t *)realloc(g_seq, (size_t)nc * sizeof(uint32_t));
    g_seq = t;
    g_cap = nc;
}

static void g_next(void)
{
    idx_t len = g_len;
    g_ensure(len + 1);
    uint32_t best = 0;
    for (idx_t L = 1; L <= len; ++L) {
        idx_t start = len - L;
        uint32_t count = 1;
        while (start >= L) {
            bool match = true;
            for (idx_t j = 0; j < L; ++j) {
                if (g_seq[start + j] != g_seq[start - L + j]) { match = false; break; }
            }
            if (!match) break;
            ++count;
            start -= L;
        }
        if (count > best) best = count;
    }
    g_seq[len] = best;
    g_len = len + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (g_len < 1) {
        g_ensure(2);
        g_seq[0] = 1;
        g_len = 1;
    }
    while (g_len < n) g_next();
    return (seq_t)(g_seq[n - 1] & 1u);
}
