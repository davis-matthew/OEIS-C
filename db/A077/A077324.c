/*
 * A077324  Sum of terms of n-th row of A077321.
 * https://oeis.org/A077324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of A077321, where row n holds the n smallest
 *            primes == 1 (mod n) that do not appear in any earlier row. The
 *            rows are built one after another using a growing prime sieve
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static uint8_t *s_comp = NULL;   /* s_comp[i] != 0 -> i is not prime */
static uint8_t *s_used = NULL;   /* s_used[i] != 0 -> prime i already used */
static uint64_t s_lim = 0;
static idx_t s_done = 0;         /* number of rows computed */
static seq_t *s_res = NULL;      /* s_res[m] = a(m) */
static idx_t s_rescap = 0;

static void ensure_limit(uint64_t newlim)
{
    uint64_t i, j;
    uint8_t *nc, *nu;
    if (newlim <= s_lim) return;
    nc = (uint8_t *)malloc((size_t)(newlim + 1));
    nu = (uint8_t *)malloc((size_t)(newlim + 1));
    memset(nc, 0, (size_t)(newlim + 1));
    memset(nu, 0, (size_t)(newlim + 1));
    if (s_used) {
        memcpy(nu, s_used, (size_t)(s_lim + 1));
        free(s_used);
    }
    if (s_comp) free(s_comp);
    nc[0] = 1;
    if (newlim >= 1) nc[1] = 1;
    for (i = 2; i * i <= newlim; i++) {
        if (!nc[i]) {
            for (j = i * i; j <= newlim; j += i) nc[j] = 1;
        }
    }
    s_comp = nc;
    s_used = nu;
    s_lim = newlim;
}

static void compute_row(idx_t m)
{
    idx_t count = 0;
    seq_t sum = 0;
    uint64_t p = (uint64_t)m + 1;
    while (count < m) {
        while (p > s_lim) ensure_limit(s_lim * 2);
        if (!s_comp[p] && !s_used[p]) {
            s_used[p] = 1;
            sum += (seq_t)p;
            count++;
        }
        p += m;
    }
    s_res[m] = sum;
}

seq_t a(idx_t n)
{
    if (s_lim == 0) ensure_limit((uint64_t)1 << 16);
    if (n + 1 > s_rescap) {
        idx_t nc = s_rescap ? s_rescap : 64;
        while (nc < n + 1) nc *= 2;
        if (s_res) s_res = mem_resize(seq_t, s_res, nc);
        else s_res = mem_new(seq_t, nc);
        s_rescap = nc;
    }
    while (s_done < n) {
        s_done++;
        compute_row(s_done);
    }
    return s_res[n];
}
