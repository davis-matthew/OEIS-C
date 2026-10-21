/*
 * A214981  Number of terms in the greedy Lucas-and-Fibonacci representations
 *          of 1,2,...,n; partial sums of A214973.
 * https://oeis.org/A214981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} t(k) – the cumulative number of terms in
 *            greedy Lucas‑and‑Fibonacci representations of the integers 1..n.
 *            t(k) counts how many basis elements (the sorted set of Fibonacci
 *            and Lucas numbers) are used to represent k
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_largest
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* qsort */
#include "../utils/find_largest.h"

static int cmp_seq_t_cb(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* ---------- build the basis B up to n ---------- */
    const size_t MAX_BAS = 200;           /* plenty for n ≤ 10⁴ */
    seq_t basis[MAX_BAS];
    size_t cnt = 0;

    /* Fibonacci numbers: 1,1,2,3,5,... */
    seq_t f1 = 1, f2 = 1;
    if (cnt < MAX_BAS) basis[cnt++] = f1; /* 1 */
    if (cnt < MAX_BAS) basis[cnt++] = f2; /* 1 */
    while (1) {
        seq_t f3 = f1 + f2;
        if (f3 > (seq_t)n) break;
        if (cnt < MAX_BAS) basis[cnt++] = f3;
        f1 = f2;
        f2 = f3;
    }

    /* Lucas numbers: 2,1,3,4,7,11,... */
    seq_t l1 = 2, l2 = 1;
    if (cnt < MAX_BAS) basis[cnt++] = l1; /* 2 */
    if (cnt < MAX_BAS) basis[cnt++] = l2; /* 1 */
    while (1) {
        seq_t l3 = l1 + l2;
        if (l3 > (seq_t)n) break;
        if (cnt < MAX_BAS) basis[cnt++] = l3;
        l1 = l2;
        l2 = l3;
    }

    /* sort and deduplicate */
    qsort(basis, cnt, sizeof(seq_t), cmp_seq_t_cb);
    size_t uniq = 0;
    if (cnt > 0) {
        uniq = 1;
        basis[0] = basis[0];
        for (size_t i = 1; i < cnt; ++i) {
            if (basis[i] != basis[uniq - 1]) {
                if (uniq < MAX_BAS) basis[uniq++] = basis[i];
            }
        }
    }

    /* ---------- compute a(n) via greedy representations ---------- */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t rem = k;
        seq_t terms = 0;
        while (rem > 0) {
            size_t idx = find_largest(basis, uniq, rem);
            rem -= basis[idx];
            ++terms;
        }
        result += terms;
    }
    return result;
}
