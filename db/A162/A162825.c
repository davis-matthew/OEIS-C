/*
 * A162825  Positive numbers k such that 60*k/(60-k) are integers.
 * https://oeis.org/A162825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162825: positive numbers k such that 60*k/(60-k) is an integer. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static storage for the pre‑computed sequence values. */
    static seq_t terms[70];      /* 68 terms, extra space for safety */
    static idx_t terms_len = 0;
    static bool initialized = false;

    if (!initialized)
    {
        /* Temporary array for candidate values. */
        seq_t cand[200];
        idx_t ccount = 0;

        /* Explicitly add k = 60. */
        cand[ccount++] = (seq_t)60;

        /* Iterate over all positive divisors of 3600. */
        for (uint32_t d = 1; d <= 3600; ++d)
        {
            if (3600U % d == 0U)
            {
                /* k = 60 - d, valid only if positive. */
                seq_t k1 = (seq_t)60 - (seq_t)d;
                if (k1 > (seq_t)0)
                    cand[ccount++] = k1;

                /* k = 60 + d, always positive. */
                seq_t k2 = (seq_t)60 + (seq_t)d;
                if (k2 > (seq_t)0)
                    cand[ccount++] = k2;
            }
        }

        /* Sort and deduplicate the candidate list. */
        qsort(cand, (size_t)ccount, sizeof(seq_t), cmp_seq);

        terms_len = 0;
        idx_t i = 0;
        while (i < ccount)
        {
            seq_t val = cand[i];
            terms[terms_len++] = val;
            /* Skip duplicate values. */
            while (i < ccount && cand[i] == val) ++i;
        }

        initialized = true;
    }

    if (n == 0 || n > terms_len)
        return (seq_t)0;      /* Undefined for indices outside the range. */

    return terms[n - 1];
}
