/*
 * A094878  n sets a new record in A094830.
 * https://oeis.org/A094878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th k at which
 *            A094830(k) sets a new record (strictly greater than all earlier
 *            values). The code sieves k in blocks and fills in A094830 from
 *            the top of each block down, using A094830(k) = 0 if k is prime,
 *            else 1 + A094830(k + sumsq(digits(k)))
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

#define A094878_B ((uint64_t)1 << 20)
#define A094878_M ((uint64_t)1024)
#define A094878_L (A094878_B + A094878_M)
#define A094878_MAXREC 4096

static uint16_t A094878_sq4[10000];
static int A094878_sq4_init = 0;

static void A094878_init_sq4(void)
{
    int i;
    if (A094878_sq4_init) return;
    for (i = 0; i < 10000; i++) {
        int x = i, s = 0;
        while (x) { int d = x % 10; s += d * d; x /= 10; }
        A094878_sq4[i] = (uint16_t)s;
    }
    A094878_sq4_init = 1;
}

static uint64_t A094878_sumsq(uint64_t x)
{
    uint64_t s = 0;
    while (x) { s += A094878_sq4[x % 10000]; x /= 10000; }
    return s;
}

static uint32_t A094878_direct(uint64_t x)
{
    uint32_t c = 0;
    while (!is_prime((idx_t)x)) { x += A094878_sumsq(x); ++c; }
    return c;
}

static uint32_t *A094878_bp = NULL;
static size_t A094878_bpcnt = 0;
static uint64_t A094878_bplim = 0;

static void A094878_ensure_base(uint64_t need)
{
    uint64_t lim, i, j;
    uint8_t *cmp;
    if (need <= A094878_bplim) return;
    lim = need * 2 + 100;
    cmp = (uint8_t *)malloc((size_t)lim + 1);
    memset(cmp, 0, (size_t)lim + 1);
    free(A094878_bp);
    A094878_bp = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 2));
    A094878_bpcnt = 0;
    for (i = 2; i <= lim; i++) {
        if (cmp[i]) continue;
        A094878_bp[A094878_bpcnt++] = (uint32_t)i;
        for (j = i * i; j <= lim; j += i) cmp[j] = 1;
    }
    free(cmp);
    A094878_bplim = lim;
}

static uint64_t A094878_recs[A094878_MAXREC];
static unsigned A094878_cnt = 0;
static uint64_t A094878_next = 0;
static uint32_t A094878_best = 0;
static uint32_t *A094878_steps = NULL;
static uint8_t *A094878_comp = NULL;

static void A094878_block(void)
{
    uint64_t lo = A094878_next, hi = lo + A094878_L;
    uint64_t r = 0, i;
    size_t t;
    uint32_t *st = A094878_steps;
    uint8_t *cp = A094878_comp;

    /* r = floor(sqrt(hi)) */
    {
        uint64_t x = hi, y = (uint64_t)1 << 32;
        while (y > x / y + 1 || y * y > x) y = (y + x / y) / 2;
        while ((y + 1) * (y + 1) <= x) y++;
        while (y * y > x) y--;
        r = y;
    }
    A094878_ensure_base(r + 1);

    memset(cp, 0, (size_t)A094878_L);
    if (lo == 0) { cp[0] = 1; cp[1] = 1; }
    for (t = 0; t < A094878_bpcnt; t++) {
        uint64_t p = A094878_bp[t], s;
        if (p * p >= hi) break;
        s = ((lo + p - 1) / p) * p;
        if (s < p * p) s = p * p;
        for (; s < hi; s += p) cp[s - lo] = 1;
    }

    for (i = A094878_L; i-- > 0;) {
        uint64_t k = lo + i;
        if (!cp[i]) { st[i] = 0; continue; }
        {
            uint64_t s = k + A094878_sumsq(k);
            if (s < hi) st[i] = 1 + st[s - lo];
            else st[i] = 1 + A094878_direct(s);
        }
    }

    for (i = (lo == 0 ? 1 : 0); i < A094878_B; i++) {
        if (st[i] > A094878_best) {
            A094878_best = st[i];
            if (A094878_cnt < A094878_MAXREC)
                A094878_recs[A094878_cnt++] = lo + i;
        }
    }
    A094878_next = lo + A094878_B;
}

seq_t a(idx_t n)
{
    A094878_init_sq4();
    if (!A094878_steps) {
        A094878_steps = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)A094878_L);
        A094878_comp = (uint8_t *)malloc((size_t)A094878_L);
    }
    if (n < 1 || n > A094878_MAXREC) return (seq_t)0;
    while ((uint64_t)A094878_cnt < (uint64_t)n) A094878_block();
    return (seq_t)A094878_recs[(size_t)(n - 1)];
}
