/*
 * A003068  Problimes (third definition).
 * https://oeis.org/A003068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1) = 2; a(n) = a(n-1) + ceiling(1 / Product_{i=1..n-1} (1 -
 *            1/a(i))), computed exactly with internal multi-limb integers D =
 *            Prod a(i), N = Prod (a(i)-1)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_big, cmp_big, limb_at, top_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/bitlen_big.h"
#include "../utils/cmp_big.h"
#include "../utils/limb_at.h"
#include "../utils/top_bits.h"

static uint32_t *bD = NULL, *bN = NULL, *bT = NULL;
static size_t lD = 0, lN = 0, bcap = 0;
static uint64_t *av = NULL;
static idx_t alen = 0, acap = 0;

static void ensure_cap(size_t need) {
    if (need <= bcap) return;
    size_t nc = bcap ? bcap : 64;
    while (nc < need) nc *= 2;
    bD = (uint32_t *)realloc(bD, nc * sizeof(uint32_t));
    bN = (uint32_t *)realloc(bN, nc * sizeof(uint32_t));
    bT = (uint32_t *)realloc(bT, nc * sizeof(uint32_t));
    bcap = nc;
}

/* out = x * m, returns length (normalized) */
static size_t mul_small(uint32_t *out, const uint32_t *x, size_t len, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        out[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry) {
        out[len++] = (uint32_t)carry;
        carry >>= 32;
    }
    while (len > 0 && out[len - 1] == 0) len--;
    return len;
}

static void extend_to(idx_t n) {
    if (n + 1 > acap) {
        idx_t nc = acap ? acap : 1024;
        while (nc < n + 1) nc *= 2;
        av = (uint64_t *)realloc(av, nc * sizeof(uint64_t));
        acap = nc;
    }
    if (alen == 0) {
        ensure_cap(64);
        av[1] = 2;
        bD[0] = 2; lD = 1;
        bN[0] = 1; lN = 1;
        alen = 1;
    }
    while (alen < n) {
        size_t bl = bitlen_big(bD, lD);
        size_t s = bl > 62 ? bl - 62 : 0;
        uint64_t Dt = top_bits(bD, lD, s);
        uint64_t Nt = top_bits(bN, lN, s);
        uint64_t q = Dt / (Nt + 1);
        int exact = 0;
        ensure_cap(lD + 4);
        for (;;) {
            size_t tl = mul_small(bT, bN, lN, (uint32_t)(q + 1));
            int c = cmp_big(bT, tl, bD, lD);
            if (c > 0) break;
            q++;
            if (c == 0) { exact = 1; break; }
        }
        if (!exact) {
            size_t tl = mul_small(bT, bN, lN, (uint32_t)q);
            exact = (cmp_big(bT, tl, bD, lD) == 0);
        }
        uint64_t next = av[alen] + q + (exact ? 0 : 1);
        alen++;
        av[alen] = next;
        ensure_cap(lD + 4);
        lD = mul_small(bD, bD, lD, (uint32_t)next);
        lN = mul_small(bN, bN, lN, (uint32_t)(next - 1));
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > alen) extend_to(n);
    return (seq_t)av[n];
}
