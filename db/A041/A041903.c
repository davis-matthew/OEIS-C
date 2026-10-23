/*
 * A041903  Denominators of continued fraction convergents to sqrt(473).
 * https://oeis.org/A041903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 174*a(n-4) - a(n-8) (n>7) Initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=4, a(4)=171, a(5)=175, a(6)=521, a(7)=696
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *buf = NULL;
    static size_t  buf_cap = 0;
    static size_t  computed_upto = 7;          /* last index already computed */

    /* First eight values */
    static const seq_t init[8] = {1, 1, 3, 4, 171, 175, 521, 696};

    /* Direct return for the first eight terms */
    if (n < 8)
        return init[n];

    /* Allocate or grow the buffer to hold indices up to n */
    if (buf_cap <= (size_t)n)
    {
        size_t new_cap = (size_t)n + 1;
        if (buf == NULL)
            buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));

        if (buf == NULL)
            abort(); /* allocation failed */

        /* Initialise the first eight terms */
        for (size_t i = 0; i < 8; ++i)
            buf[i] = init[i];
        buf_cap = new_cap;
    }

    /* Compute any terms that are missing up to n */
    for (size_t i = computed_upto + 1; i <= (size_t)n; ++i)
    {
        /* Recurrence: a(i) = 174*a(i-4) - a(i-8)   (i ≥ 8) */
        buf[i] = 174 * buf[i - 4] - buf[i - 8];
    }
    computed_upto = (size_t)n;

    return buf[n];
}
