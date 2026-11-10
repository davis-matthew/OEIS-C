/*
 * A229744  Values of records in A096303.
 * https://oeis.org/A229744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   search -- a(n) = n-th record value of A096303(m), m = 1, 2, 3,
 *            ... * A096303(m) is the number of steps of x -> x + popcount(x)
 *            before x enters * the trajectory of 1 (A010062). It is computed
 *            for consecutive ranges of m * from the top down with a 128-entry
 *            ring buffer, because f(m) = 1 + f(m + pop...
 * Range:     u8 n <= 25, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned A229744_pc(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned)__builtin_popcountll((unsigned long long)x);
#else
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

/* checkpoints: every 1024th element of the trajectory of 1 */
static uint64_t *A229744_cp = NULL;
static size_t A229744_ncp = 0, A229744_capcp = 0;
static uint64_t A229744_gval = 1, A229744_gidx = 0;

static void A229744_cp_extend(uint64_t target)
{
    if (!A229744_cp) {
        A229744_capcp = 1024;
        A229744_cp = (uint64_t *)malloc(A229744_capcp * sizeof(uint64_t));
        if (!A229744_cp) abort();
        A229744_cp[0] = 1;
        A229744_ncp = 1;
        A229744_gval = 1;
        A229744_gidx = 0;
    }
    while (A229744_gval <= target) {
        A229744_gval += A229744_pc(A229744_gval);
        ++A229744_gidx;
        if ((A229744_gidx & 1023u) == 0) {
            if (A229744_ncp == A229744_capcp) {
                A229744_capcp <<= 1;
                A229744_cp = (uint64_t *)realloc(A229744_cp, A229744_capcp * sizeof(uint64_t));
                if (!A229744_cp) abort();
            }
            A229744_cp[A229744_ncp++] = A229744_gval;
        }
    }
}

/* an element of the trajectory of 1 that is <= x (x >= 1) */
static uint64_t A229744_cp_start(uint64_t x)
{
    A229744_cp_extend(x);
    size_t lo = 0, hi = A229744_ncp;   /* largest i with cp[i] <= x */
    while (hi - lo > 1) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (A229744_cp[mid] <= x) lo = mid; else hi = mid;
    }
    return A229744_cp[lo];
}

/* direct computation of A096303(m) by walking both trajectories */
static uint32_t A229744_direct(uint64_t m)
{
    uint64_t s = A229744_cp_start(m), x = m;
    uint32_t c = 0;
    while (s != x) {
        if (s < x) s += A229744_pc(s);
        else { x += A229744_pc(x); ++c; }
    }
    return c;
}

static uint64_t *A229744_first = NULL;   /* first position of each value, 0 = none */
static size_t A229744_fcap = 0;
static uint32_t A229744_maxv = 0;

#define A229744_B ((uint64_t)1 << 16)

static void A229744_segment(uint64_t lo, uint64_t hi)
{
    static uint8_t mark[1 << 16];
    uint32_t ring[128];
    uint64_t bhi = hi;
    memset(ring, 0, sizeof(ring));
    for (;;) {
        uint64_t blo = (bhi - lo + 1 > A229744_B) ? bhi - A229744_B + 1 : lo;
        memset(mark, 0, (size_t)(bhi - blo + 1));
        uint64_t s = A229744_cp_start(blo);
        while (s < blo) s += A229744_pc(s);
        while (s <= bhi) { mark[s - blo] = 1; s += A229744_pc(s); }
        uint64_t m;
        for (m = bhi; ; --m) {
            uint32_t f;
            if (mark[m - blo]) f = 0;
            else {
                uint64_t nx = m + A229744_pc(m);
                if (nx > hi) f = 1 + A229744_direct(nx);
                else f = ring[nx & 127] + 1;
            }
            ring[m & 127] = f;
            if ((size_t)f >= A229744_fcap) {
                size_t nc = A229744_fcap ? A229744_fcap : 256;
                while (nc <= (size_t)f) nc <<= 1;
                A229744_first = (uint64_t *)realloc(A229744_first, nc * sizeof(uint64_t));
                if (!A229744_first) abort();
                memset(A229744_first + A229744_fcap, 0, (nc - A229744_fcap) * sizeof(uint64_t));
                A229744_fcap = nc;
            }
            if (A229744_first[f] == 0 || m < A229744_first[f]) A229744_first[f] = m;
            if (f > A229744_maxv) A229744_maxv = f;
            if (m == blo) break;
        }
        if (blo == lo) break;
        bhi = blo - 1;
    }
}

static uint32_t *A229744_rec = NULL;
static size_t A229744_nrec = 0;
static uint64_t A229744_L = 0;

static void A229744_records(void)
{
    size_t cnt = 0;
    uint64_t curmin = UINT64_MAX;
    uint32_t v;
    free(A229744_rec);
    A229744_rec = (uint32_t *)malloc(((size_t)A229744_maxv + 1) * sizeof(uint32_t));
    if (!A229744_rec) abort();
    for (v = A229744_maxv; ; --v) {
        uint64_t p = A229744_first[v];
        if (p != 0 && p < curmin) {
            A229744_rec[cnt++] = v;
            curmin = p;
        }
        if (v == 0) break;
    }
    /* collected descending; reverse to ascending */
    {
        size_t i, j;
        for (i = 0, j = cnt ? cnt - 1 : 0; i < j; ++i, --j) {
            uint32_t t = A229744_rec[i]; A229744_rec[i] = A229744_rec[j]; A229744_rec[j] = t;
        }
    }
    A229744_nrec = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A229744_nrec < (size_t)n) {
        uint64_t newL = A229744_L ? A229744_L * 2 : ((uint64_t)1 << 16);
        A229744_segment(A229744_L + 1, newL);
        A229744_L = newL;
        A229744_records();
    }
    return (seq_t)A229744_rec[n - 1];
}
