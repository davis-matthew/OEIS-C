/*
 * A191890  Ordered sequence of nonnegative differences c*f-d*g, where f and g
 *          are positive Fibonacci numbers (A000045) and {c,d}={3,5}.
 * https://oeis.org/A191890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑negative value of 3*f−5*g or 5*f−3*g, where
 *            f,g are positive Fibonacci numbers. The sequence is sorted in
 *            non‑decreasing order
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compare function for qsort: returns negative if *a < *b, zero otherwise. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* Build (or extend) a static cache of the sequence values.
   After the call, *out will point to an array of distinct,
   sorted non‑negative differences, and *len will contain its
   length.  The cache will contain at least n elements. */
static void build_sequence(idx_t n, seq_t **out, size_t *len)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t fib_limit = 0;

    /* If we already have enough values, just return. */
    if (cache && cache_len >= (size_t)n) {
        *out = cache;
        *len = cache_len;
        return;
    }

    /* Free old cache if it exists; we will rebuild anew. */
    if (cache) {
        free(cache);
        cache = NULL;
        cache_len = 0;
    }

    /* Start with a moderate Fibonacci limit; double it until we
       obtain at least n distinct values. */
    if (fib_limit == 0) fib_limit = 80;          /* heuristic */
    while (1) {
        /* Allocate Fibonacci numbers up to fib_limit. */
        size_t fl = fib_limit;
        seq_t *fibs = (seq_t *)malloc((fl + 1) * sizeof(seq_t));
        if (!fibs) exit(1);
        fibs[0] = 1;              /* unused, but convenient */
        fibs[1] = 1;
        for (size_t i = 2; i <= fl; ++i)
            fibs[i] = fibs[i-1] + fibs[i-2];

        /* Estimate number of candidate differences: at most 2*fl^2. */
        size_t cap = 2 * fl * fl + 16;
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tmp) { free(fibs); exit(1); }
        size_t cnt = 0;
        seq_t zero = 0;

        for (size_t i = 1; i <= fl; ++i) {
            for (size_t j = 1; j <= fl; ++j) {
                seq_t t1 = fibs[i] * 3 - fibs[j] * 5;
                if (t1 >= zero) {
                    if (cnt == cap) {
                        cap *= 2;
                        tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                        if (!tmp) { free(fibs); exit(1); }
                    }
                    tmp[cnt++] = t1;
                }
                seq_t t2 = fibs[i] * 5 - fibs[j] * 3;
                if (t2 >= zero) {
                    if (cnt == cap) {
                        cap *= 2;
                        tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                        if (!tmp) { free(fibs); exit(1); }
                    }
                    tmp[cnt++] = t2;
                }
            }
        }
        free(fibs);

        /* Sort the array. */
        qsort(tmp, cnt, sizeof(seq_t), cmp_seq);

        /* Deduplicate in place. */
        size_t uniq = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (i == 0 || tmp[i] != tmp[i-1]) {
                tmp[uniq++] = tmp[i];
            }
        }

        if (uniq >= (size_t)n) {
            cache = tmp;
            cache_len = uniq;
            *out = cache;
            *len = cache_len;
            return;
        }

        /* Not enough values: free temp and extend Fibonacci limit. */
        free(tmp);
        fib_limit *= 2;
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety for 0‑based callers */
    seq_t *arr;
    size_t len;
    build_sequence(n, &arr, &len);
    return arr[n-1];         /* sequence is 1‑based: a(1) is first element */
}
