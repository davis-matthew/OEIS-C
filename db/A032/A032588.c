/*
 * A032588  Lucky numbers ending with digit 7.
 * https://oeis.org/A032588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th lucky number whose last digit is 7.
   The function builds a lucky sieve up to a dynamic limit and caches
   the lucky numbers ending in 7.  For each request it ensures the
   cache contains at least n elements, enlarging the sieve limit if
   necessary. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

#ifdef __cplusplus
/* C++: use new/delete for object construction/destruction. */

#else
/* C: use malloc/free. */

#endif

/* --------------------------------------------------------------------- */
/* Global state for the memoised table of lucky numbers ending with 7.   */
static seq_t  *lucky7_arr = NULL;   /* array of lucky numbers ending with 7 */
static size_t lucky7_cnt = 0;       /* number of such lucky numbers */
static size_t lucky_limit = 200000; /* current sieve limit */

/* --------------------------------------------------------------------- */
/* Build a lucky list up to ‘limit’ and collect those ending in 7.      */
static void init_lucky7(void)
{
    /* Number of odd numbers up to lucky_limit. */
    size_t len = (lucky_limit + 1) / 2;

    seq_t *lucky = alloc_seq(len);
    seq_t *tmp   = alloc_seq(len);
    if (!lucky || !tmp) { /* out of memory */
        if (lucky) free_seq(lucky);
        if (tmp)   free_seq(tmp);
        exit(EXIT_FAILURE);
    }

    /* Fill odd numbers: 1, 3, 5, … */
    for (size_t i = 0; i < len; ++i)
        lucky[i] = (seq_t)(2 * (seq_t)i + 1);

    /* Lucky sieve – remove every step‑th element. */
    for (size_t i = 1; i < len; ++i)
    {
        size_t step = (size_t)lucky[i];
        if (step > len) break;   /* no more eliminations */

        size_t newlen = 0;
        for (size_t j = 0; j < len; ++j)
        {
            if ((j + 1) % step != 0)   /* keep non‑multiples of step */
                tmp[newlen++] = lucky[j];
        }

        /* swap buffers and update length */
        seq_t *swap = lucky;
        lucky      = tmp;
        tmp        = swap;
        len        = newlen;
    }

    /* Collect lucky numbers that end with digit 7. */
    seq_t *temp = alloc_seq(len);
    if (!temp) { free_seq(lucky); free_seq(tmp); exit(EXIT_FAILURE); }

    size_t count = 0;
    for (size_t i = 0; i < len; ++i)
    {
        if (lucky[i] % 10 == 7)
            temp[count++] = lucky[i];
    }

    free_seq(lucky);
    free_seq(tmp);

    lucky7_arr = temp;
    lucky7_cnt = count;
}

/* --------------------------------------------------------------------- */
/* Public function returning the n‑th lucky number ending with digit 7. */
seq_t a(idx_t n)
{
    /* Lazy initialization. */
    if (!lucky7_arr)
        init_lucky7();

    /* Grow the sieve if the cache does not contain enough terms. */
    while ((idx_t)n > (idx_t)lucky7_cnt)
    {
        lucky_limit *= 2;          /* increase the sieve range   */
        free_seq(lucky7_arr);      /* discard old cache          */
        init_lucky7();             /* rebuild with larger limit  */
    }

    /* Indices start at 1. */
    return lucky7_arr[(size_t)(n - 1)];
}
