/*
 * A083507  Triangle read by rows where the n-th row contains all distinct
 *          numbers that can be obtained as the product of three distinct
 *          numbers chosen from [1..n].
 * https://oeis.org/A083507
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑3)‑th element of the flattened triangle of
 *            distinct products of three distinct numbers chosen from [1..r],
 *            row by row. The triangle is generated row by row; for each r the
 *            distinct unordered triple products i<j<k (1≤i<j<k≤r) are
 *            collected, sorted ascending, and appended to a global...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* local comparator for qsort returning <0,0,>0 for seq_t values */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* behavior below the offset is irrelevant */
    if (n < 3)
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t  csize = 0;       /* number of cached terms */
    static size_t  ccap  = 0;       /* capacity of cache */
    static idx_t   next_r = 3;      /* next row to generate */

    /* generate rows until we have enough terms */
    while (csize < (size_t)(n - 2))
    {
        idx_t r = next_r;

        /* number of unordered triples from 1..r */
        idx_t triples = r * (r - 1) * (r - 2) / 6;

        seq_t *tmp = (seq_t *)malloc(triples * sizeof(seq_t));
        if (!tmp)
            return 0;        /* allocation failed */

        size_t pos = 0;
        for (idx_t i = 1; i <= r - 2; ++i)
            for (idx_t j = i + 1; j <= r - 1; ++j)
                for (idx_t k = j + 1; k <= r; ++k)
                    tmp[pos++] = (seq_t)i * (seq_t)j * (seq_t)k;

        /* sort and deduplicate */
        qsort(tmp, triples, sizeof(seq_t), seq_cmp);
        size_t uniq = 0;
        for (size_t p = 0; p < triples; ++p)
        {
            if (uniq == 0 || tmp[p] != tmp[uniq - 1])
                tmp[uniq++] = tmp[p];
        }

        /* enlarge cache if necessary */
        size_t needed = csize + uniq;
        if (needed > ccap)
        {
            size_t new_cap = ccap ? ccap * 2 : 128;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr)
                return 0;    /* allocation failed */
            cache = new_ptr;
            ccap  = new_cap;
        }

        /* copy unique products into cache */
        for (size_t t = 0; t < uniq; ++t)
            cache[csize + t] = tmp[t];
        csize += uniq;

        free(tmp);
        ++next_r;
    }

    return cache[n - 3];
}
