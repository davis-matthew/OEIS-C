/*
 * A031961  Numbers with exactly three distinct base-9 digits.
 * https://oeis.org/A031961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate all base‑9 numbers with exactly three distinct digits
   (length 3 to 5) and return the nth value (1‑based). */
#include <stdlib.h>
#include <stdint.h>

/* Helper: comparison function for qsort */
static int compare_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t*)p;
    seq_t b = *(const seq_t*)q;
    return (a < b) ? -1 : (a > b);
}

/* Cached sequence buffer */
static seq_t *cache = NULL;      /* buffer for generated numbers */
static size_t cached_len = 0;    /* number of valid entries in cache */

/* Populate the cache on first call */
static void generate_cache(void)
{
    if (cache != NULL)
        return;                 /* already generated */

    /* Max possible entries: L=3,4,5 with 84 digit sets each */
    const size_t max_cap = 20000;
    cache = (seq_t*)malloc(max_cap * sizeof(seq_t));
    if (!cache)
        exit(1);                /* allocation failure */

    size_t idx = 0;             /* next free position */

    /* Loop over all 3‑digit sets {d1,d2,d3}, d1<d2<d3 */
    for (int d1 = 0; d1 < 9; ++d1)
        for (int d2 = d1 + 1; d2 < 9; ++d2)
            for (int d3 = d2 + 1; d3 < 9; ++d3)
            {
                uint8_t S[3] = { (uint8_t)d1, (uint8_t)d2, (uint8_t)d3 };

                /* Length 3 */
                for (int a = 0; a < 3; ++a)
                    if (S[a] != 0)
                        for (int b = 0; b < 3; ++b)
                            for (int c = 0; c < 3; ++c)
                            {
                                int mask = (1 << a) | (1 << b) | (1 << c);
                                if (mask != 7) /* not all 3 digits used */
                                    continue;
                                seq_t val = (seq_t)S[a];
                                val = val * 9 + (seq_t)S[b];
                                val = val * 9 + (seq_t)S[c];
                                cache[idx++] = val;
                            }

                /* Length 4 */
                for (int a = 0; a < 3; ++a)
                    if (S[a] != 0)
                        for (int b = 0; b < 3; ++b)
                            for (int c = 0; c < 3; ++c)
                                for (int d = 0; d < 3; ++d)
                                {
                                    int mask = (1 << a) | (1 << b) | (1 << c) | (1 << d);
                                    if (mask != 7)
                                        continue;
                                    seq_t val = 0;
                                    val = val * 9 + (seq_t)S[a];
                                    val = val * 9 + (seq_t)S[b];
                                    val = val * 9 + (seq_t)S[c];
                                    val = val * 9 + (seq_t)S[d];
                                    cache[idx++] = val;
                                }

                /* Length 5 */
                for (int a = 0; a < 3; ++a)
                    if (S[a] != 0)
                        for (int b = 0; b < 3; ++b)
                            for (int c = 0; c < 3; ++c)
                                for (int d = 0; d < 3; ++d)
                                    for (int e = 0; e < 3; ++e)
                                    {
                                        int mask = (1 << a) | (1 << b) | (1 << c) |
                                                   (1 << d) | (1 << e);
                                        if (mask != 7)
                                            continue;
                                        seq_t val = 0;
                                        val = val * 9 + (seq_t)S[a];
                                        val = val * 9 + (seq_t)S[b];
                                        val = val * 9 + (seq_t)S[c];
                                        val = val * 9 + (seq_t)S[d];
                                        val = val * 9 + (seq_t)S[e];
                                        cache[idx++] = val;
                                    }
            }

    /* Sort the generated values */
    qsort(cache, idx, sizeof(seq_t), compare_seq);
    cached_len = idx;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, but return 0 */

    generate_cache();          /* ensure cache is ready */

    if ((size_t)n > cached_len)
        return 0;   /* out of range, but sequence defined only for n >= 1 */

    return cache[n - 1];
}
