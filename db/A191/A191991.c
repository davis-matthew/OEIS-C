/*
 * A191991  Ordered sequence of nonnegative differences c*f-d*g, where f and g
 *          are Lucas numbers (A000032 beginning at 1) and {c,d}={2,5}.
 * https://oeis.org/A191991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest nonnegative value of 2*L_i - 5*L_j or
 *            5*L_i - 2*L_j, where L_k is the k-th Lucas number (L_1 = 1, L_2
 *            = 3, L_3 = 4, …). 0 is included as the smallest element
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* minimal comparator for seq_t */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Return the n-th element of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    /* determine a suitable upper bound for Lucas indices */
    const size_t MAX_IDX = (sizeof(seq_t) <= sizeof(uint64_t)) ? 80 : 200;

    /* regenerate the sequence if needed */
    if (seq_len < (size_t)n) {
        /* allocate vector for Lucas numbers */
        size_t i_max = MAX_IDX;
        seq_t *lucas = (seq_t *)malloc(sizeof(seq_t) * (i_max + 1));
        if (!lucas) return (seq_t)0;  /* out of memory, fallback */

        /* L_0 = 2, L_1 = 1; we use 1-based indices only */
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        for (size_t i = 2; i <= i_max; ++i)
            lucas[i] = lucas[i - 1] + lucas[i - 2];

        /* estimate upper bound for number of candidates */
        size_t cand_cap = i_max * i_max * 2 + 5;
        seq_t *cand = (seq_t *)malloc(sizeof(seq_t) * cand_cap);
        if (!cand) { free(lucas); return (seq_t)0; }

        size_t cand_len = 0;
        for (size_t i = 1; i <= i_max; ++i) {
            for (size_t j = 1; j <= i_max; ++j) {
                /* 2*L_i - 5*L_j */
                if ((seq_t)2 * lucas[i] >= (seq_t)5 * lucas[j])
                    cand[cand_len++] = (seq_t)2 * lucas[i] - (seq_t)5 * lucas[j];
                /* 5*L_i - 2*L_j */
                if ((seq_t)5 * lucas[i] >= (seq_t)2 * lucas[j])
                    cand[cand_len++] = (seq_t)5 * lucas[i] - (seq_t)2 * lucas[j];
            }
        }
        /* Always include 0 as the first element */
        cand[cand_len++] = (seq_t)0;

        free(lucas);

        /* sort and deduplicate */
        qsort(cand, cand_len, sizeof(seq_t), cmp_seq_t);
        size_t unique_len = 0;
        for (size_t i = 0; i < cand_len; ++i) {
            if (i == 0 || cand[i] != cand[i - 1])
                cand[unique_len++] = cand[i];
        }

        /* replace old sequence */
        free(seq);
        seq = cand;
        seq_len = unique_len;
    }

    return seq[n - 1];
}
