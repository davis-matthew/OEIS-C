/*
 * A235341  Positive integers a such that there exist b, c with 1/a + 1/b +
 *          1/c = 1/3 and a >= b >= c.
 * https://oeis.org/A235341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   cmp_seq_t_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of positive integers a such that there exist b,c with
   1/a + 1/b + 1/c = 1/3 and a >= b >= c.
   The set of solutions is finite: {9,12,15,18,20,21,24,28,30,
   36,42,45,48,60,84,120,156}. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t_local.h"

/* static storage for the solutions in ascending order */
static seq_t  terms[32];
static size_t terms_cnt = 0;
static bool   initialized = false;

/* generate the complete list of solutions once */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* enumerate over c = 4 … 9 (only these give solutions) */
    for (idx_t c = 4; c <= 9; ++c)
    {
        /* b ranges from c up to floor(6*c/(c-3)) */
        idx_t bmax = (6 * c) / (c - 3);

        for (idx_t b = c; b <= bmax; ++b)
        {
            /* t = b*(c-3) - 3*c must be positive */
            idx_t t_int = b * (c - 3) - 3 * c;
            if (t_int <= 0) continue;

            seq_t t   = (seq_t)t_int;                      /* positive divisor */
            seq_t num = (seq_t)(3 * (b * c));             /* 3*b*c """

            /* a is integer iff t divides 3*b*c */
            if (num % t != 0) continue;

            seq_t a_val = num / t;
            if (a_val < (seq_t)b) continue;              /* enforce a >= b */

            /* store a_val (duplicates will be removed later) */
            if (terms_cnt < sizeof(terms)/sizeof(terms[0]))
                terms[terms_cnt++] = a_val;
        }
    }

    /* sort the array and remove duplicates */
    if (terms_cnt > 1)
        qsort(terms, terms_cnt, sizeof(seq_t), cmp_seq_t_local);

    size_t uniq = 0;
    for (size_t i = 0; i < terms_cnt; ++i)
    {
        if (uniq == 0 || cmp_seq_t_local(&terms[i], &terms[uniq-1]) != 0)
            terms[uniq++] = terms[i];
    }
    terms_cnt = uniq;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > terms_cnt) return (seq_t)0;   /* out of range */
    return terms[n - 1];
}
