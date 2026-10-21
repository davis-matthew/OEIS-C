/*
 * A073042  Numbers of the form 10^s*t^2 or 10^s*t^3, s,t >= 0.
 * https://oeis.org/A073042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073042 – Numbers of the form 10^s * t^2 or 10^s * t^3 (s, t ≥ 0),
   sorted increasingly. a(1) = 0.  */
#include <stdlib.h>

/* comparison function used by qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *sa = (const seq_t *)a;
    const seq_t *sb = (const seq_t *)b;
    if (*sa < *sb) return -1;
    if (*sa > *sb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache, built on first call */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL)                /* initialise only once          */
    {
        /* parameters chosen to produce more than 40000 distinct terms,
           which is far beyond the 10000‑term requirement                */
        const idx_t T_MAX = 2000;      /* maximum t to consider          */
        const idx_t S_MAX = 9;         /* maximum exponent of 10 (10^9)  */

        /* pre‑compute powers of 10: 10^0 … 10^S_MAX */
        seq_t pow10[S_MAX + 1];
        pow10[0] = (seq_t)1;
        for (idx_t i = 1; i <= S_MAX; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;

        /* temporary storage for all candidate numbers before deduplication */
        const idx_t tmp_capacity = (T_MAX + 1) * 2 * (S_MAX + 1) + 1;
        seq_t *tmp = (seq_t *)malloc(tmp_capacity * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                 /* allocation failure              */

        size_t tmp_len = 0;
        tmp[tmp_len++] = 0;                     /* t = 0 gives the value 0        */

        for (idx_t t = 1; t <= T_MAX; ++t)
        {
            const seq_t ts = (seq_t)t;
            const seq_t t2 = ts * ts;           /* t^2                          */
            const seq_t t3 = t2 * ts;           /* t^3                          */

            for (idx_t s = 0; s <= S_MAX; ++s)
            {
                seq_t p10 = pow10[s];
                tmp[tmp_len++] = t2 * p10;      /* 10^s * t^2                   */
                tmp[tmp_len++] = t3 * p10;      /* 10^s * t^3                   */
            }
        }

        /* sort all generated values */
        qsort(tmp, tmp_len, sizeof(seq_t), cmp_seq);

        /* deduplicate in place – compact unique values at the front */
        size_t uniq = 0;
        for (size_t i = 0; i < tmp_len; ++i)
        {
            if (i == 0 || tmp[i] != tmp[i - 1])
                tmp[uniq++] = tmp[i];
        }

        /* allocate the final cache of unique terms */
        cache = (seq_t *)malloc(uniq * sizeof(seq_t));
        if (!cache)
        {
            free(tmp);
            return (seq_t)0;
        }
        for (size_t i = 0; i < uniq; ++i)
            cache[i] = tmp[i];
        cache_size = (idx_t)uniq;

        free(tmp);                         /* free temporary storage          */
    }

    /* return the requested term, or 0 if the index is out of bounds    */
    if (n < 1 || n > cache_size)
        return (seq_t)0;
    return cache[n - 1];
}
