/*
 * A058695  Number of ways to partition 2n+1 into positive integers.
 * https://oeis.org/A058695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(2*n + 1), where p(m) is the partition function
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 63, u64 n <= 207, u128 n <= 728,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are supplied by the build system */

static seq_t *p = NULL;        /* dynamic array for partition numbers */
static size_t alloc_len = 0;   /* current allocated length of p   */
static idx_t max_computed = (idx_t)-1;   /* largest index for which p is known */

/* Extend the array to hold at least m+1 entries.  Allocate larger
   sizes if needed to reduce reallocations. */
static void ensure_capacity(idx_t m)
{
    if (m + 1 <= alloc_len)
        return;

    size_t new_len = (size_t)(m + 1);
    /* Allocate more space (roughly 1.5× to reduce churn). */
    if (new_len < alloc_len * 3 / 2 + 16)
        new_len = alloc_len * 3 / 2 + 16;

    seq_t *new_p = (seq_t *)realloc(p, new_len * sizeof(seq_t));
    if (!new_p)                /* allocation failure – abort for now */
        return;
    p = new_p;
    /* Initialise the new slots to zero. */
    for (size_t i = alloc_len; i < new_len; ++i)
        p[i] = 0;
    alloc_len = new_len;
}

/* Compute partition numbers p[0..m] using the pentagonal number theorem. */
static void compute_partitions(idx_t m)
{
    /* First-time initialization: p[0] = 1. */
    if (max_computed == (idx_t)-1) {
        ensure_capacity(0);
        p[0] = 1;
        max_computed = 0;
    }

    if (m <= max_computed)
        return;

    ensure_capacity(m);

    /* Compute p[i] for i = max_computed+1 .. m */
    for (idx_t i = max_computed + 1; i <= m; ++i) {
        seq_t sumPos = 0;
        seq_t sumNeg = 0;

        /* Generalized pentagonal numbers: k(3k±1)/2 */
        for (int k = 1; ; ++k) {
            int g1 = k * (3 * k - 1) / 2;
            if (g1 > (int)i)
                break;
            if (k & 1)
                sumPos += p[i - g1];
            else
                sumNeg += p[i - g1];

            int g2 = k * (3 * k + 1) / 2;
            if (g2 <= (int)i) {
                if (k & 1)
                    sumPos += p[i - g2];
                else
                    sumNeg += p[i - g2];
            }
        }
        /* All terms are non‑negative; the final subtraction yields p[i]. */
        p[i] = sumPos - sumNeg;
    }
    max_computed = m;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute the required partition argument: 2*n + 1. */
    seq_t m_seq = (seq_t)n * (seq_t)2 + (seq_t)1;
    idx_t m = (idx_t)m_seq;

    compute_partitions(m);
    return p[m];
}
