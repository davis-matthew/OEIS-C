/*
 * A094204  Duplicate of A078584.
 * https://oeis.org/A094204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = prime(2n) - prime(2n-1) (computed by incremental
 *            trial-division prime list)
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A094204_pl = NULL;
static size_t A094204_cnt = 0;
static size_t A094204_cap = 0;
static uint64_t A094204_next = 3;

static bool A094204_isp(uint64_t p)
{
    for (size_t i = 0; i < A094204_cnt; ++i) {
        uint64_t q = A094204_pl[i];
        if (q * q > p) break;
        if (p % q == 0) return false;
    }
    return true;
}

static bool A094204_grow(size_t target)
{
    if (A094204_cnt >= target) return true;
    if (A094204_cap < target) {
        size_t nc = target * 2 + 16;
        uint64_t *np = (uint64_t *)realloc(A094204_pl, nc * sizeof(uint64_t));
        if (!np) return false;
        A094204_pl = np;
        A094204_cap = nc;
    }
    if (A094204_cnt == 0) {
        A094204_pl[0] = 2;
        A094204_cnt = 1;
        A094204_next = 3;
    }
    while (A094204_cnt < target) {
        uint64_t p = A094204_next;
        if (A094204_isp(p)) A094204_pl[A094204_cnt++] = p;
        A094204_next += 2;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t need = (size_t)(2 * n);
    if (!A094204_grow(need)) return (seq_t)0;
    uint64_t d = A094204_pl[need - 1] - A094204_pl[need - 2];
    return (seq_t)d;
}
