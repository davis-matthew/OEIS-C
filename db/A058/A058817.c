/*
 * A058817  Even cototient numbers.
 * https://oeis.org/A058817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th even cototient number; that is, * the n‑th
 *            smallest non‑negative integer m such that * m = N - φ(N) for
 *            some integer N and m is even. * The sequence begins 0, 2, 4, 6,
 *            8, 12, 14, …
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 80 terms through n = 9548 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static data kept for subsequent calls */
    static seq_t *terms = NULL;          /* sorted, distinct even cototients */
    static size_t terms_size = 0;        /* number of terms computed */
    static const size_t LIMIT = 1000000; /* upper bound for precomputation */

    /* lazy initialization */
    if (terms == NULL)
    {
        /* ---------- step 1: compute φ up to LIMIT ---------- */
        uint64_t *phi = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
        if (!phi) return (seq_t)0; /* memory failure */
        for (size_t i = 0; i <= LIMIT; ++i) phi[i] = i;

        for (size_t p = 2; p <= LIMIT; ++p)
        {
            if (phi[p] == p)           /* p is prime */
            {
                for (size_t j = p; j <= LIMIT; j += p)
                    phi[j] -= phi[j] / p;
            }
        }

        /* ---------- step 2: collect even cototients ---------- */
        size_t cap = LIMIT;            /* temporary array capacity */
        seq_t *list = mem_new(seq_t, cap);
        if (!list) { free(phi); return (seq_t)0; }
        size_t cnt = 0;
        for (size_t i = 1; i <= LIMIT; ++i)
        {
            seq_t m = (seq_t)i - (seq_t)phi[i];
            if ((m % 2) == 0)          /* only even */
            {
                if (cnt == cap) /* extend if needed (unlikely) */
                {
                    cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, list, cap);
                    if (!tmp) { mem_free(list); free(phi); return (seq_t)0; }
                    list = tmp;
                }
                list[cnt++] = m;
            }
        }

        /* ---------- step 3: sort and deduplicate ---------- */
        mem_qsort(list, cnt, sizeof(seq_t), cmp_seq_t);

        /* count unique values */
        terms_size = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (i == 0 || list[i] != list[i - 1])
                ++terms_size;

        terms = mem_new(seq_t, terms_size);
        if (!terms) { mem_free(list); free(phi); return (seq_t)0; }

        size_t idx = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (i == 0 || list[i] != list[i - 1])
                terms[idx++] = list[i];

        /* cleanup */
        mem_free(list);
        free(phi);
    }

    /* ---------- step 4: answer query ---------- */
    if (n == 0) return (seq_t)0;                /* safety */
    idx_t z = n - 1;                           /* zero‑based index */
    if ((size_t)z >= terms_size) return (seq_t)0; /* out of range */
    return terms[z];
}
