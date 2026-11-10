/*
 * A073536  Breaking indices for A058842 (i.e., n such that A058842(n) is not
 *          equal to 3*A058842 (n-1) ).
 * https://oeis.org/A073536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   let b(1)=1, b(k)=3*b(k-1) mod 2^k (A058842); a(n) is the n-th
 *            index k>=2 where b(k) != 3*b(k-1), i.e. where 3*b(k-1) >= 2^k.
 *            Computed by simulating b with multi-limb arithmetic
 * Range:     u8 n <= 53, u16 n <= 198, u32 n <= 198, u64 n <= 198,
 *            u128 n <= 198, big any n
 * Validated: standard, pass, 72 terms through n = 198 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint32_t *bx_limbs = NULL;
static size_t bx_cap = 0;
static uint64_t bx_m = 0;
static uint64_t *bx_res = NULL;
static size_t bx_cnt = 0, bx_rcap = 0;

static void bx_extend(size_t need)
{
    while (bx_cnt < need) {
        if (bx_m == 0) {
            bx_cap = 8;
            bx_limbs = (uint32_t *)calloc(bx_cap, sizeof(uint32_t));
            bx_limbs[0] = 1;
            bx_m = 1;
        }
        uint64_t k = bx_m + 1;
        size_t topl = (size_t)(k / 32);
        if (topl + 2 > bx_cap) {
            size_t nc = bx_cap * 2;
            while (nc < topl + 2) nc *= 2;
            uint32_t *t = (uint32_t *)realloc(bx_limbs, nc * sizeof(uint32_t));
            memset(t + bx_cap, 0, (nc - bx_cap) * sizeof(uint32_t));
            bx_limbs = t;
            bx_cap = nc;
        }
        uint64_t carry = 0;
        for (size_t i = 0; i < bx_cap; i++) {
            uint64_t v = (uint64_t)bx_limbs[i] * 3u + carry;
            bx_limbs[i] = (uint32_t)v;
            carry = v >> 32;
        }
        int brk = 0;
        unsigned sh = (unsigned)(k % 32);
        uint32_t keep = (sh == 0) ? 0u : (((uint32_t)1 << sh) - 1u);
        if (bx_limbs[topl] & ~keep) brk = 1;
        bx_limbs[topl] &= keep;
        for (size_t i = topl + 1; i < bx_cap; i++) {
            if (bx_limbs[i]) brk = 1;
            bx_limbs[i] = 0;
        }
        bx_m = k;
        if (brk) {
            if (bx_cnt == bx_rcap) {
                bx_rcap = bx_rcap ? bx_rcap * 2 : 64;
                bx_res = (uint64_t *)realloc(bx_res, bx_rcap * sizeof(uint64_t));
            }
            bx_res[bx_cnt++] = k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    bx_extend((size_t)n);
    return (seq_t)bx_res[n - 1];
}
