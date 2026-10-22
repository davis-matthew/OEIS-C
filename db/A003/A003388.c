/*
 * A003388  Sum of 10 nonzero 8th powers.
 * https://oeis.org/A003388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. Enumerate nondecreasing 10-tuples of positive bases
 *            whose 8th powers sum to at most a bound X, then sort and remove
 *            duplicates. If fewer than n sums fit, double X and search again.
 *            a(n) is the n-th smallest sum
 * Range:     u8 n <= 1, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3388_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3388_cmp.h"

static uint64_t *a3388_vals = 0;
static size_t a3388_nvals = 0, a3388_cap = 0;
static uint64_t a3388_P[300];
static int a3388_np = 0;

static void a3388_push(uint64_t v)
{
    if (a3388_nvals == a3388_cap) {
        a3388_cap = a3388_cap ? a3388_cap * 2 : 4096;
        a3388_vals = (uint64_t *)realloc(a3388_vals, a3388_cap * sizeof(uint64_t));
    }
    a3388_vals[a3388_nvals++] = v;
}

static void a3388_gen(int k, int b, uint64_t s, uint64_t X)
{
    int i;
    if (k == 0) {
        a3388_push(s);
        return;
    }
    for (i = b; i < a3388_np; i++) {
        if (a3388_P[i] > (X - s) / (uint64_t)k) break;
        a3388_gen(k - 1, i, s + a3388_P[i], X);
    }
}

seq_t a(idx_t n)
{
    static uint64_t X = 0;
    static size_t count = 0;
    while ((uint64_t)n > (uint64_t)count) {
        uint64_t b;
        size_t i, w;
        X = X ? X * 2 : 65536;
        a3388_np = 0;
        for (b = 1; b < 256; b++) {
            uint64_t p = b * b;
            p = p * p;
            p = p * p;
            if (p > X) break;
            a3388_P[a3388_np++] = p;
        }
        a3388_nvals = 0;
        a3388_gen(10, 0, 0, X);
        qsort(a3388_vals, a3388_nvals, sizeof(uint64_t), a3388_cmp);
        w = 0;
        for (i = 0; i < a3388_nvals; i++) {
            if (w == 0 || a3388_vals[i] != a3388_vals[w - 1])
                a3388_vals[w++] = a3388_vals[i];
        }
        count = w;
    }
    return (seq_t)a3388_vals[n - 1];
}
