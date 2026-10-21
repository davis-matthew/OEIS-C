/*
 * A205565  Number of ways of writing n = u + v with u <= v, and u,v having in
 *          ternary representation no 3.
 * https://oeis.org/A205565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (u, v) with u + v = n, u ≤ v,
 *            and both u, v have no digit 2 in base‑3 (i.e. digits only 0 or
 *            1)
 * Range:     u8 n <= 9840, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compare two idx_t values for qsort. */
static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

/* Binary search in a sorted array of idx_t. */
static bool binary_search_idx(const idx_t *arr, size_t sz, idx_t val)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < sz) && (arr[lo] == val);
}

seq_t a(idx_t n)
{
    /* a(0) = 1 (the pair 0+0). */
    if (n == 0)
        return (seq_t)1;

    /* Generate all Cantor numbers ≤ n. */
    size_t cap = 16;
    idx_t *c = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!c)
        return (seq_t)0;                  /* unlikely, but safe */
    size_t sz = 1;
    c[0] = 0;                              /* include zero */

    for (size_t i = 0; i < sz; ++i) {
        idx_t x = c[i];

        /* child y1 = 3*x */
        if (x <= n / 3) {
            idx_t y1 = x * 3;
            if (y1 != x) {
                if (sz == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(c, cap * sizeof(idx_t));
                    if (!tmp) { free(c); return (seq_t)0; }
                    c = tmp;
                }
                c[sz++] = y1;
            }
        }

        /* child y2 = 3*x + 1 */
        if (x <= (n - 1) / 3) {
            idx_t y2 = x * 3 + 1;
            if (y2 != x) {
                if (sz == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(c, cap * sizeof(idx_t));
                    if (!tmp) { free(c); return (seq_t)0; }
                    c = tmp;
                }
                c[sz++] = y2;
            }
        }
    }

    /* Sort to enable binary search. */
    qsort(c, sz, sizeof(idx_t), cmp_idx);

    /* Count unordered pairs (u, v) with u + v = n, u ≤ v. */
    idx_t half = n >> 1;
    seq_t count = 0;
    for (size_t i = 0; i < sz; ++i) {
        idx_t u = c[i];
        if (u > half)
            break;                          /* u ≤ v ensured */
        idx_t v = n - u;
        if (binary_search_idx(c, sz, v))
            ++count;
    }

    free(c);
    return count;
}
