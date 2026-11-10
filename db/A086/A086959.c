/*
 * A086959  Greatest m with A086956(m)=n.
 * https://oeis.org/A086959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the 3rd occurrence of n in A086956 (A086956(m)=n
 *            for at most 3 values of m); computed by direct simulation of
 *            A086956
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A086959_cnt = NULL;
static uint64_t *A086959_third = NULL;
static uint64_t A086959_cap = 0;
static uint64_t A086959_m = 0;

static void A086959_grow(uint64_t need)
{
    if (need <= A086959_cap) return;
    uint64_t nc = A086959_cap ? A086959_cap : 64;
    while (nc < need) nc <<= 1;
    uint32_t *c = (uint32_t *)realloc(A086959_cnt, nc * sizeof(uint32_t));
    if (!c) abort();
    A086959_cnt = c;
    uint64_t *t = (uint64_t *)realloc(A086959_third, nc * sizeof(uint64_t));
    if (!t) abort();
    A086959_third = t;
    memset(A086959_cnt + A086959_cap, 0, (nc - A086959_cap) * sizeof(uint32_t));
    memset(A086959_third + A086959_cap, 0, (nc - A086959_cap) * sizeof(uint64_t));
    A086959_cap = nc;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A086959_grow((uint64_t)n + 2);
    while (A086959_third[n] == 0) {
        uint64_t m = A086959_m + 1;
        A086959_grow(m + 2);
        uint64_t d;
        for (d = 1; d <= m; ++d) {
            if (m % d == 0 && A086959_cnt[d] <= 2) break;
        }
        A086959_cnt[d]++;
        if (A086959_cnt[d] == 3) A086959_third[d] = m;
        A086959_m = m;
    }
    return (seq_t)A086959_third[n];
}
