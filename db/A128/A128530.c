/*
 * A128530  Nontrivial products of three lucky numbers.
 * https://oeis.org/A128530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128530: Nontrivial products of three lucky numbers
 * Formula: a(n) = n-th distinct value of { L[i]*L[j]*L[k] | i,j,k ≥ 0 }
 *          where L are lucky numbers > 1 (A000959 without the leading 1).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* compare two seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

/* generate the first `need` lucky numbers > 1 (skip the initial 1) */
static void generate_lucky(uint32_t *out, size_t need, size_t *cnt)
{
    size_t limit = 1000;          /* starting limit of odd numbers */
    while (1)
    {
        /* build list of odd numbers up to limit */
        size_t list_len = limit / 2 + 1;
        size_t *list = (size_t *)malloc(list_len * sizeof(size_t));
        size_t len = 0;
        for (uint32_t i = 1; i <= limit; i += 2)
            list[len++] = i;      /* 1,3,5,… */

        /* sieve elimination to produce lucky numbers */
        size_t idx = 1;           /* skip the first element 1 itself */
        while (idx < len)
        {
            size_t step = list[idx];
            if (step > len) break;
            size_t write = 0;
            for (size_t read = 0; read < len; ++read)
            {
                if ((read + 1) % step != 0)
                    list[write++] = list[read];
            }
            len = write;
            ++idx;
        }

        /* copy lucky numbers > 1 into out[] */
        size_t outlen = 0;
        for (size_t i = 1; i < len && outlen < need; ++i)
            out[outlen++] = (uint32_t)list[i];

        free(list);

        if (outlen >= need)
        {
            *cnt = outlen;
            return;
        }
        limit *= 2;          /* increase limit and try again */
    }
}

/* static storage for memoized sequence */
static seq_t   *uniq_vals   = NULL;
static size_t  uniq_cnt     = 0;
static bool    initialized  = false;

/* main sequence function, external linkage */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds */

    if (!initialized)
    {
        /* ---------- generate and memoize the sequence ---------- */
        const size_t NEED_LUCKY   = 80;   /* enough to produce >10 000 unique values */
        uint32_t lucky[NEED_LUCKY];
        size_t lucky_cnt = 0;
        generate_lucky(lucky, NEED_LUCKY, &lucky_cnt);

        /* count all unordered triples i≤j≤k */
        size_t total = lucky_cnt * (lucky_cnt + 1) * (lucky_cnt + 2) / 6;
        seq_t *all = (seq_t *)malloc(total * sizeof(seq_t));

        size_t pos = 0;
        for (size_t i = 0; i < lucky_cnt; ++i)
            for (size_t j = i; j < lucky_cnt; ++j)
                for (size_t k = j; k < lucky_cnt; ++k)
                {
                    seq_t val = (seq_t)lucky[i] *
                                (seq_t)lucky[j] *
                                (seq_t)lucky[k];
                    all[pos++] = val;
                }

        /* sort all products */
        qsort(all, total, sizeof(seq_t), cmp_seq);

        /* compress unique values */
        uniq_vals = (seq_t *)malloc(total * sizeof(seq_t));
        uniq_cnt  = 0;
        for (size_t i = 0; i < total; ++i)
        {
            if (uniq_cnt == 0 || all[i] != uniq_vals[uniq_cnt - 1])
                uniq_vals[uniq_cnt++] = all[i];
        }

        free(all);
        initialized = true;
    }

    if (n <= uniq_cnt)
        return uniq_vals[n - 1];        /* 1‑based indexing */
    return 0;                            /* n too large */
}
