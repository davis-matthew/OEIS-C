/*
 * A111222  Integers that can be expressed as the sum of 4 factorials.
 * https://oeis.org/A111222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A111222: integers representable as a! + b! + c! + d! with
   a, b, c, d >= 0.  The function generates all possible sums of four
   factorials whose factorial values fit in the type seq_t, sorts, removes
   duplicates, and returns the n‑th term (0‑based). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *unique_sums = NULL;
static idx_t unique_cnt = 0;

/* Comparison function used by qsort (works for built‑in integer types
   and for arbitrary‑precision integer classes that overload < >). */
static int compare_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    return (*va < *vb) ? -1 : (*va > *vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (!initialized)
    {
        /* --- compute factorials up to the largest that does not overflow --- */
        const int MAX_PRECALC = 30;          /* safety upper bound */
        seq_t fact[MAX_PRECALC + 1];
        fact[0] = 1;
        int max_f = 0;
        for (int i = 1; i <= MAX_PRECALC; ++i)
        {
            seq_t next = fact[i - 1] * (seq_t)i;
            /* Stop if multiplication wrapped around (overflow). */
            if (next < fact[i - 1])
                break;
            fact[i] = next;
            max_f = i;
        }

        /* --- total number of combinations with repetition: C(max_f+4,4) --- */
        size_t comb_cnt = 1;
        for (int k = 1; k <= 4; ++k)
            comb_cnt = comb_cnt * (size_t)(max_f + k) / (size_t)k;

        seq_t *combs = (seq_t *)malloc(comb_cnt * sizeof(seq_t));
        if (!combs)
            exit(1);

        /* --- generate all sums with a <= b <= c <= d --- */
        size_t idx = 0;
        for (int i = 0; i <= max_f; ++i)
            for (int j = i; j <= max_f; ++j)
                for (int k = j; k <= max_f; ++k)
                    for (int l = k; l <= max_f; ++l)
                    {
                        combs[idx++] = fact[i] + fact[j] + fact[k] + fact[l];
                    }
        /* idx should equal comb_cnt */

        /* --- sort the sums --- */
        qsort(combs, comb_cnt, sizeof(seq_t), compare_seq);

        /* --- deduplicate --- */
        unique_sums = (seq_t *)malloc(comb_cnt * sizeof(seq_t));
        if (!unique_sums)
            exit(1);

        if (comb_cnt > 0)
        {
            unique_sums[0] = combs[0];
            unique_cnt = 1;
            for (size_t i = 1; i < comb_cnt; ++i)
            {
                if (compare_seq(&combs[i], &combs[i - 1]) != 0)
                {
                    unique_sums[unique_cnt++] = combs[i];
                }
            }
        }
        free(combs);
        initialized = true;
    }

    if (n >= unique_cnt)
        return 0; /* undefined for out of range; caller may handle otherwise */
    return unique_sums[n];
}
