/*
 * A134812  a(n) = 2a(n-2) + 4a(n-3), n >= 3.
 * https://oeis.org/A134812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 35, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-2) + 4*a(n-3) (n ≥ 4),
   a(0) = 0, a(1) = 1, a(2) = -1, a(3) = 1 */

#include <stdlib.h>

/* The function keeps a static cache so that any order of calls works. */
seq_t a(idx_t n)
{
    /* dynamic array of computed terms  */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;   /* number of terms currently stored   */
    static size_t seq_cap = 0;   /* allocated capacity                 */

    /* initialise cache with the first 4 terms */
    if (!seq)
    {
        seq_cap = 4;
        seq_len = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)-1;
        seq[3] = (seq_t)1;
    }

    /* return directly if the requested index is already cached */
    if ((size_t)n < seq_len)
        return seq[(size_t)n];

    /* assure capacity up to the requested index */
    size_t need = (size_t)n + 1;
    if (need > seq_cap)
    {
        size_t new_cap = seq_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        seq_cap = new_cap;
    }

    /* grow the cache */
    for (size_t i = seq_len; i < need; ++i)
    {
        if (i < 4)              /* initial terms are known */
            continue;

        /* recurrence: a(n) = 2*a(n-2) + 4*a(n-3) */
        seq[i] = (seq_t)2 * seq[i - 2] + (seq_t)4 * seq[i - 3];
    }
    seq_len = need;

    return seq[(size_t)n];
}
