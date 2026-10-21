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
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

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
        seq_t *list = (seq_t *)malloc(cap * sizeof(seq_t));
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
                    seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                    if (!tmp) { free(list); free(phi); return (seq_t)0; }
                    list = tmp;
                }
                list[cnt++] = m;
            }
        }

        /* ---------- step 3: sort and deduplicate ---------- */
        qsort(list, cnt, sizeof(seq_t), cmp_seq_t);

        /* count unique values */
        terms_size = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (i == 0 || list[i] != list[i - 1])
                ++terms_size;

        terms = (seq_t *)malloc(terms_size * sizeof(seq_t));
        if (!terms) { free(list); free(phi); return (seq_t)0; }

        size_t idx = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (i == 0 || list[i] != list[i - 1])
                terms[idx++] = list[i];

        /* cleanup */
        free(list);
        free(phi);
    }

    /* ---------- step 4: answer query ---------- */
    if (n == 0) return (seq_t)0;                /* safety */
    idx_t z = n - 1;                           /* zero‑based index */
    if ((size_t)z >= terms_size) return (seq_t)0; /* out of range */
    return terms[z];
}
