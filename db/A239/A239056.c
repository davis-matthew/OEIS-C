/*
 * A239056  Sum of the parts in the partitions of 4n into 4 parts with
 *          smallest part = 1.
 * https://oeis.org/A239056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8) (valid for n ≥ 9; a(1)=4, a(2)=32,
 *            a(3)=120, a(4)=304, a(5)=600, a(6)=1056, a(7)=1708, a(8)=2560)
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *table = NULL;   /* cache of computed terms, 1‑based */
static idx_t  computed = 0;   /* highest n for which table[n] is valid */

/* ensure the cache holds indices 1..n (at least up to 8) */
static void init_cache(idx_t n)
{
    idx_t need = (n > 8 ? n : 8) + 1;               /* +1 for unused 0 slot */
    if (table == NULL) {
        table = (seq_t *)malloc(need * sizeof(seq_t));
        if (table == NULL) abort();

        /* seed the first eight terms */
        table[1] = 4;
        table[2] = 32;
        table[3] = 120;
        table[4] = 304;
        table[5] = 600;
        table[6] = 1056;
        table[7] = 1708;
        table[8] = 2560;
        computed = 8;
    } else if (need > computed + 1) {
        seq_t *newtab = (seq_t *)realloc(table, need * sizeof(seq_t));
        if (newtab == NULL) abort();
        table = newtab;
        /* keep already known terms; computed stays unchanged */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) abort();           /* OEIS offset: a(1) is first term */

    init_cache(n);

    /* compute any missing terms up to n using the given recurrence
       (prevents unsigned underflow by grouping positives and negatives
       separately) */
    while (computed < n) {
        idx_t i = computed + 1;

        seq_t pos = 2 * table[i-1]            /* + 2*a(n-1)   */
                 + 2 * table[i-3]            /* + 2*a(n-3)   */
                 + 2 * table[i-5]            /* + 2*a(n-5)   */
                 + 2 * table[i-7];           /* + 2*a(n-7)   */

        seq_t neg = table[i-2]               /*   a(n-2)     */
                 + 4 * table[i-4]            /*   4*a(n-4)   */
                 + table[i-6]               /*   a(n-6)     */
                 + table[i-8];               /*   a(n-8)     */

        table[i] = pos - neg;
        computed = i;
    }

    return table[n];
}
