/*
 * A035681  Number of partitions of n into parts 8k+1 and 8k+4 with at least
 *          one part of each type.
 * https://oeis.org/A035681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k+1 and 8k+4 * with
 *            at least one part of each type. * Computed by dynamic
 *            programming: count all partitions using those * parts, then
 *            subtract those that use only 1‑mod‑8 parts or only * 4‑mod‑8
 *            parts
 * Range:     u8 n <= 52, u16 n <= 141, u32 n <= 441, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
    /* In C++ we use new[] / delete[] for proper constructors/destructors. */
    #define ALLOC_SEQ_T(N)  new seq_t[(N)]
    #define FREE_SEQ_T(P)   delete[] (P)
#else
    /* In plain C we use malloc() / free().  seq_t is a plain integer type. */
    #define ALLOC_SEQ_T(N)  ((seq_t*)malloc((N) * sizeof(seq_t)))
    #define FREE_SEQ_T(P)   free(P)
#endif

/* cached maximum n for which the tables have been computed */
static idx_t cached_max_n = 0;
/* dynamic programming tables */
static seq_t *dp_total = NULL;   /* partitions using 1‑ or 4‑mod‑8 parts */
static seq_t *dp_s1    = NULL;   /* partitions using only 1‑mod‑8 parts */
static seq_t *dp_s4    = NULL;   /* partitions using only 4‑mod‑8 parts */

/* recompute tables up to given n (n >= 0) */
static void recompute(idx_t n)
{
    size_t len = (size_t)n + 1;

    /* allocate new tables */
    seq_t *new_total = ALLOC_SEQ_T(len);
    seq_t *new_s1    = ALLOC_SEQ_T(len);
    seq_t *new_s4    = ALLOC_SEQ_T(len);
    if (!new_total || !new_s1 || !new_s4) {
        /* memory allocation failure – abort safely */
        abort();
    }

    /* initialise all counts to zero */
    for (size_t i = 0; i < len; ++i) {
        new_total[i] = 0;
        new_s1[i]    = 0;
        new_s4[i]    = 0;
    }
    new_total[0] = 1;
    new_s1[0]    = 1;
    new_s4[0]    = 1;

    /* process parts ≡ 1 (mod 8) */
    for (idx_t part = 1; part <= n; part += 8) {
        for (idx_t s = part; s <= n; ++s) {
            new_total[s] += new_total[s - part];
            new_s1[s]    += new_s1[s - part];
        }
    }

    /* process parts ≡ 4 (mod 8) */
    for (idx_t part = 4; part <= n; part += 8) {
        for (idx_t s = part; s <= n; ++s) {
            new_total[s] += new_total[s - part];
            new_s4[s]    += new_s4[s - part];
        }
    }

    /* free old tables */
    FREE_SEQ_T(dp_total);
    FREE_SEQ_T(dp_s1);
    FREE_SEQ_T(dp_s4);

    /* store new tables */
    dp_total = new_total;
    dp_s1    = new_s1;
    dp_s4    = new_s4;
    cached_max_n = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)       /* offset 1: below offset is undefined */
        return 0;

    if (n > cached_max_n)
        recompute(n);

    /* partitions that use at least one part of each type */
    return dp_total[n] - dp_s1[n] - dp_s4[n];
}
