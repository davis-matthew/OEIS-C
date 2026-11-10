/*
 * A075646  (Sum of n-th group in A075643)/(n+1).
 * https://oeis.org/A075646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of n-th group of A075643)/(n+1); group n is built
 *            greedily: slot i<n takes the smallest unused multiple of i, slot
 *            n takes the smallest unused multiple of n making the group sum
 *            divisible by n+1
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static unsigned char *g_used = NULL;
static uint64_t g_cap = 0;
static uint64_t g_ptr[4096];
static uint64_t *g_res = NULL;
static uint64_t g_done = 0;
static uint64_t g_rescap = 0;

static bool g_is_used(uint64_t x)
{
    return x < g_cap && g_used[x];
}

static void g_mark(uint64_t x)
{
    if (x >= g_cap) {
        uint64_t nc = g_cap ? g_cap : 1024;
        while (nc <= x) nc <<= 1;
        unsigned char *p = (unsigned char *)realloc(g_used, (size_t)nc);
        if (!p) abort();
        for (uint64_t i = g_cap; i < nc; ++i) p[i] = 0;
        g_used = p;
        g_cap = nc;
    }
    g_used[x] = 1;
}

static void g_build_group(uint64_t n)
{
    uint64_t sum = 0, i;
    for (i = 1; i < n; ++i) {
        uint64_t k = g_ptr[i] ? g_ptr[i] : 1;
        while (g_is_used(i * k)) k++;
        g_ptr[i] = k;
        g_mark(i * k);
        sum += i * k;
    }
    uint64_t m = n + 1;
    uint64_t r = (m - sum % m) % m;      /* need sum + x == 0 mod m */
    uint64_t k = (m - r) % m;            /* k*n == -k mod m, need -k == r */
    if (k == 0) k = m;
    while (g_is_used(n * k)) k += m;
    g_mark(n * k);
    sum += n * k;

    if (g_done + 1 >= g_rescap) {
        uint64_t nc = g_rescap ? g_rescap * 2 : 128;
        uint64_t *p = (uint64_t *)realloc(g_res, (size_t)(nc * sizeof(uint64_t)));
        if (!p) abort();
        g_res = p;
        g_rescap = nc;
    }
    g_res[++g_done] = sum / m;
}

seq_t a(idx_t n)
{
    while (g_done < (uint64_t)n) g_build_group(g_done + 1);
    return (seq_t)g_res[n];
}
