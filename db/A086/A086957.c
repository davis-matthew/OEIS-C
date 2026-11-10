/*
 * A086957  Smallest m with A086956(m)=n.
 * https://oeis.org/A086957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A086956(m) = n; found by simulating
 *            A086956 (smallest divisor of m used at most twice before) until
 *            value n first appears
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t *A086957_cnt = NULL;   /* occurrence counts by value */
static uint64_t *A086957_first = NULL; /* first index where value occurs, 0 if none */
static uint64_t A086957_cap = 0;
static uint64_t A086957_len = 0;      /* terms of A086956 computed */

static void A086957_grow(uint64_t need)
{
    if (need <= A086957_cap) return;
    uint64_t nc = A086957_cap ? A086957_cap : 64;
    while (nc < need) nc <<= 1;
    uint8_t *c = (uint8_t *)realloc(A086957_cnt, nc * sizeof(uint8_t));
    if (!c) abort();
    A086957_cnt = c;
    uint64_t *f = (uint64_t *)realloc(A086957_first, nc * sizeof(uint64_t));
    if (!f) abort();
    A086957_first = f;
    memset(A086957_cnt + A086957_cap, 0, (nc - A086957_cap) * sizeof(uint8_t));
    memset(A086957_first + A086957_cap, 0, (nc - A086957_cap) * sizeof(uint64_t));
    A086957_cap = nc;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N = (uint64_t)n;
    A086957_grow(N + 2);
    while (A086957_first[N] == 0) {
        uint64_t ii = A086957_len + 1;
        A086957_grow(ii + 2);
        uint64_t best = ii;
        for (uint64_t d = 1; d * d <= ii; ++d) {
            if (ii % d == 0) {
                if (A086957_cnt[d] <= 2 && d < best) best = d;
                uint64_t d2 = ii / d;
                if (A086957_cnt[d2] <= 2 && d2 < best) best = d2;
            }
        }
        A086957_cnt[best]++;
        if (A086957_first[best] == 0) A086957_first[best] = ii;
        A086957_len = ii;
    }
    return (seq_t)A086957_first[N];
}
