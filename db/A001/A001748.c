/*
 * A001748  a(n) = 3 * prime(n).
 * https://oeis.org/A001748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*prime(n), with prime(n) found by trial division and
 *            cached
 * Range:     u8 n <= 23, u16 n <= 2450, u32 n <= 9075, u64 n <= 9075,
 *            u128 n <= 9075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *a001748_pr = NULL;
static idx_t a001748_cnt = 0;
static idx_t a001748_cap = 0;

static void a001748_extend(idx_t n)
{
    uint64_t c;
    if (a001748_cnt >= n) return;
    if (a001748_cap < n) {
        idx_t nc = a001748_cap ? a001748_cap : 64;
        uint64_t *np;
        while (nc < n) nc *= 2;
        np = (uint64_t *)realloc(a001748_pr, (size_t)nc * sizeof(uint64_t));
        if (!np) return;
        a001748_pr = np;
        a001748_cap = nc;
    }
    if (a001748_cnt == 0) {
        a001748_pr[0] = 2;
        a001748_cnt = 1;
    }
    c = a001748_pr[a001748_cnt - 1];
    while (a001748_cnt < n) {
        idx_t i;
        int ok;
        c += (c == 2) ? 1 : 2;
        ok = 1;
        for (i = 1; i < a001748_cnt; i++) {
            uint64_t p = a001748_pr[i];
            if (p * p > c) break;
            if (c % p == 0) { ok = 0; break; }
        }
        if (ok) a001748_pr[a001748_cnt++] = c;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    a001748_extend(n);
    return (seq_t)3 * (seq_t)a001748_pr[n - 1];
}
