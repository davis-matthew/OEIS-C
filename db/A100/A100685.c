/*
 * A100685  Powers of factorials A000142.
 * https://oeis.org/A100685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = all numbers of the form (k!)^m (k ≥ 1, m ≥ 1), sorted
 *            strictly increasing
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 65, u64 n <= 142,
 *            u128 n <= 305, big any n
 * Validated: standard, pass, 57 terms through n = 9026 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A100685: powers of factorials, (j!)^k, in increasing order without
 * repetition (1 = 1!^k once). All of them up to a bound 2^bits are generated,
 * sorted and deduplicated; bits doubles until there are n. The values are
 * exact: 128-bit at the fixed widths (so the order is right for every term
 * that fits), seq_t for big. (The original's bound 2^n wrapped to 0 at the
 * fixed widths and it then read past an empty heap.) */

#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef unsigned __int128 wide_t;
#endif

static int cmp_wide(const void *x, const void *y)
{
    const wide_t *a = (const wide_t *)x, *b = (const wide_t *)y;
    return *a < *b ? -1 : (*b < *a ? 1 : 0);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    for (unsigned bits = 16;; bits *= 2) {
#ifndef __cplusplus
        if (bits > 126) return (seq_t)0;          /* past 128 bits */
#endif
        wide_t B = 1;
        for (unsigned i = 0; i < bits; ++i) B *= 2;
        size_t cnt = 0, cap = 64;
        wide_t *v = mem_new(wide_t, cap);
        if (!v) abort();
        v[cnt++] = 1;
        wide_t f = 1;
        for (unsigned j = 2;; ++j) {
            if (f > B / j) break;
            f *= j;                               /* j! <= B */
            for (wide_t p = f;;) {
                if (cnt == cap) {
                    v = mem_resize(wide_t, v, cap * 2);
                    if (!v) abort();
                    cap *= 2;
                }
                v[cnt++] = p;
                if (p > B / f) break;
                p *= f;
            }
        }
        mem_qsort(v, cnt, sizeof *v, cmp_wide);
        size_t u = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (!u || !(v[i] == v[u - 1])) v[u++] = v[i];
        if (u >= (size_t)n) {
            seq_t r = (seq_t)v[n - 1];
            mem_free(v);
            return r;
        }
        mem_free(v);
    }
}
