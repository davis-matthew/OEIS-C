/*
 * A229247  Number of set partitions of {1,...,n} with largest set of size 5.
 * https://oeis.org/A229247
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B5(n) – B4(n) where Bm(n) = Σ_{k=1}^{min(m,n)}
 *            C(n−1,k−1)·Bm(n−k), Bm(0)=1. This counts partitions of an n‑set
 *            whose largest block has size exactly 5
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return binomial coefficient C(n,k) for 0 ≤ k ≤ n, using only integer
   arithmetic.  The arguments and result are of type idx_t/seq_t. */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;        /* use symmetry */
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    return r;
}

/* Number of set partitions of {1,…,n} with largest part size exactly 5. */
seq_t a(idx_t n)
{
    /* Static tables for the restricted Bell numbers B4 and B5. */
    static seq_t *B4 = NULL;
    static seq_t *B5 = NULL;
    static size_t cap4 = 0, cap5 = 0;
    static size_t len4 = 0, len5 = 0;  /* number of entries computed (indices 0 .. len-1) */

    /* --- first call: allocate and initialise --- */
    if (B4 == NULL)
    {
        cap4 = cap5 = 1;
        B4 = (seq_t *)malloc(cap4 * sizeof(seq_t));
        B5 = (seq_t *)malloc(cap5 * sizeof(seq_t));
        if (!B4 || !B5) return (seq_t)0;          /* allocation failure */
        B4[0] = B5[0] = (seq_t)1;                 /* Bm(0) = 1 */
        len4 = len5 = 1;
    }

    /* --- ensure sufficient capacity --- */
    if (cap4 <= (size_t)n)
    {
        size_t new_cap = cap4 ? cap4 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        B4 = (seq_t *)realloc(B4, new_cap * sizeof(seq_t));
        cap4 = new_cap;
    }
    if (cap5 <= (size_t)n)
    {
        size_t new_cap = cap5 ? cap5 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        B5 = (seq_t *)realloc(B5, new_cap * sizeof(seq_t));
        cap5 = new_cap;
    }

    /* --- compute restricted Bell numbers up to n --- */
    for (size_t i = len4; i <= (size_t)n; ++i)
    {
        seq_t sum = (seq_t)0;
        idx_t maxk = (i < (idx_t)4 ? (idx_t)i : (idx_t)4);
        for (idx_t k = (idx_t)1; k <= maxk; ++k)
            sum += binom_small((idx_t)i - (idx_t)1, k - (idx_t)1) *
                   B4[i - (size_t)k];
        B4[i] = sum;
    }
    len4 = (size_t)n + 1;

    for (size_t i = len5; i <= (size_t)n; ++i)
    {
        seq_t sum = (seq_t)0;
        idx_t maxk = (i < (idx_t)5 ? (idx_t)i : (idx_t)5);
        for (idx_t k = (idx_t)1; k <= maxk; ++k)
            sum += binom_small((idx_t)i - (idx_t)1, k - (idx_t)1) *
                   B5[i - (size_t)k];
        B5[i] = sum;
    }
    len5 = (size_t)n + 1;

    return B5[n] - B4[n];
}
