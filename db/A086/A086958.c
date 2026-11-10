/*
 * A086958  m such that A086956(m)=n and A086956(k)=n for exactly one k<m
 *          (k=A086957(n)).
 * https://oeis.org/A086958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index m of the second occurrence of n in A086956 (found
 *            by direct simulation of A086956)
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A086958_cnt = NULL;
static uint64_t *A086958_second = NULL;
static uint64_t A086958_cap = 0;
static uint64_t A086958_len = 0;

static void A086958_grow(uint64_t need)
{
    if (need <= A086958_cap) return;
    uint64_t nc = A086958_cap ? A086958_cap : 64;
    while (nc < need) nc <<= 1;
    uint32_t *c = (uint32_t *)realloc(A086958_cnt, nc * sizeof(uint32_t));
    if (!c) abort();
    A086958_cnt = c;
    uint64_t *s = (uint64_t *)realloc(A086958_second, nc * sizeof(uint64_t));
    if (!s) abort();
    A086958_second = s;
    memset(A086958_cnt + A086958_cap, 0, (nc - A086958_cap) * sizeof(uint32_t));
    memset(A086958_second + A086958_cap, 0, (nc - A086958_cap) * sizeof(uint64_t));
    A086958_cap = nc;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A086958_grow((uint64_t)n + 2);
    while (A086958_second[n] == 0) {
        uint64_t i = A086958_len + 1;
        A086958_grow(i + 2);
        uint64_t best = i;
        for (uint64_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                if (A086958_cnt[d] <= 2 && d < best) best = d;
                uint64_t d2 = i / d;
                if (A086958_cnt[d2] <= 2 && d2 < best) best = d2;
            }
        }
        A086958_cnt[best]++;
        if (A086958_cnt[best] == 2) A086958_second[best] = i;
        A086958_len = i;
    }
    return (seq_t)A086958_second[n];
}
