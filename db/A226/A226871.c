/*
 * A226871  Triangle read by rows: row n gives the first q divisors d(1),
 *          d(2), ..., d(q) of A225110(n) such that Sum_{i = 1..q} 1/d(i) is
 *          an integer.
 * https://oeis.org/A226871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = k-th divisor d(k) in row r, where row r lists the first q
 *            divisors of A225110(r) (q smallest >1 with Sum 1/d(i) an
 *            integer; row 1 = [1]); found by search over m with a
 *            flattened-triangle index
 * Range:     u8 n <= 185, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 98 terms through n = 9967 (2026-11-07)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

static int A226871_cmp(const void *p, const void *q)
{
    uint64_t x = *(const uint64_t*)p;
    uint64_t y = *(const uint64_t*)q;
    return (x > y) - (x < y);
}

static unsigned __int128 A226871_gcd(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* sorted divisors of m, malloc'd; returns count */
static size_t A226871_divs(uint64_t m, uint64_t **out)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    size_t cap = 16, len = 1;
    uint64_t *d = (uint64_t*)malloc(cap * sizeof(uint64_t));
    d[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        size_t cur = len;
        uint64_t power = 1;
        for (uint64_t k = 1; k <= ex[i]; ++k) {
            power *= pr[i];
            for (size_t j = 0; j < cur; ++j) {
                if (len == cap) {
                    cap <<= 1;
                    d = (uint64_t*)realloc(d, cap * sizeof(uint64_t));
                }
                d[len++] = d[j] * power;
            }
        }
    }
    qsort(d, len, sizeof(uint64_t), A226871_cmp);
    *out = d;
    return len;
}

/* smallest q>1 with integer reciprocal sum, or 0 */
static size_t A226871_q(const uint64_t *d, size_t len)
{
    unsigned __int128 num = 0, den = 1;
    for (size_t i = 0; i < len; ++i) {
        unsigned __int128 dd = d[i];
        unsigned __int128 nn = num * dd + den;
        unsigned __int128 nd = den * dd;
        unsigned __int128 g = A226871_gcd(nn, nd);
        num = nn / g;
        den = nd / g;
        if (i >= 1 && den == 1) return i + 1;
    }
    return 0;
}

static uint64_t *A226871_rm = NULL;   /* m of each row */
static uint64_t *A226871_rq = NULL;   /* row length */
static uint64_t *A226871_cum = NULL;  /* cumulative length through row */
static size_t A226871_rows = 0, A226871_cap = 0;
static uint64_t A226871_last = 0;

static void A226871_push(uint64_t m, uint64_t q)
{
    if (A226871_rows == A226871_cap) {
        A226871_cap = A226871_cap ? A226871_cap * 2 : 64;
        A226871_rm = (uint64_t*)realloc(A226871_rm, A226871_cap * sizeof(uint64_t));
        A226871_rq = (uint64_t*)realloc(A226871_rq, A226871_cap * sizeof(uint64_t));
        A226871_cum = (uint64_t*)realloc(A226871_cum, A226871_cap * sizeof(uint64_t));
    }
    uint64_t prev = A226871_rows ? A226871_cum[A226871_rows - 1] : 0;
    A226871_rm[A226871_rows] = m;
    A226871_rq[A226871_rows] = q;
    A226871_cum[A226871_rows] = prev + q;
    ++A226871_rows;
}

seq_t a(idx_t n)
{
    if (A226871_rows == 0) {
        A226871_push(1, 1);
        A226871_last = 1;
    }
    while (A226871_cum[A226871_rows - 1] < (uint64_t)n) {
        ++A226871_last;
        uint64_t *d;
        size_t len = A226871_divs(A226871_last, &d);
        size_t q = A226871_q(d, len);
        free(d);
        if (q) A226871_push(A226871_last, q);
    }
    /* binary search for first row with cum >= n */
    size_t lo = 0, hi = A226871_rows - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A226871_cum[mid] >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    uint64_t start = A226871_cum[lo] - A226871_rq[lo];
    size_t k = (size_t)((uint64_t)n - start - 1);
    uint64_t *d;
    A226871_divs(A226871_rm[lo], &d);
    uint64_t v = d[k];
    free(d);
    return (seq_t)v;
}
