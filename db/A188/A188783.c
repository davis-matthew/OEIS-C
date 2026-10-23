/*
 * A188783  Number of 8-turn bishop's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A188783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n≥1 satisfies a linear recurrence of order 17: a(n) =
 *            a(n‑17) – 5 a(n‑16) + 4 a(n‑15) + 20 a(n‑14) – 40 a(n‑13) – 16
 *            a(n‑12) + 100 a(n‑11) – 44 a(n‑10) – 110 a(n‑9) + 110 a(n‑8) +
 *            44 a(n‑7) – 100 a(n‑6) + 16 a(n‑5) + 40 a(n‑4) – 20 a(n‑3) – 4
 *            a(n‑2) + 5 a(n‑1) Initial values (for n=1…17)...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms.
   Index 0 is unused; data[1] holds a(1). */
static seq_t *data = NULL;
static idx_t  cap   = 0;
static idx_t  last  = 0;

static void ensure_capacity(idx_t n)
{
    /* Grow the cache to hold indices 0..n */
    while (cap <= n) {
        size_t new_cap = (cap == 0) ? 32 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort with zero value (unlikely in tests) */
            abort();
        }
        data = tmp;
        cap = new_cap;
    }
}

static const seq_t init_vals[18] = {
    0ULL,                         /* unused index 0 */
    0ULL,                         /* a(1) */
    0ULL,                         /* a(2) */
    0ULL,                         /* a(3) */
    384ULL,                       /* a(4) */
    88264ULL,                     /* a(5) */
    1665344ULL,                   /* a(6) */
    14497784ULL,                  /* a(7) */
    80088992ULL,                  /* a(8) */
    335122320ULL,                 /* a(9) */
    1142391712ULL,                /* a(10) */
    3358831216ULL,                /* a(11) */
    8772323808ULL,                /* a(12) */
    20882774744ULL,               /* a(13) */
    46000760736ULL,               /* a(14) */
    95075730152ULL,               /* a(15) */
    186010966464ULL,              /* a(16) */
    347367851808ULL               /* a(17) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but safe */

    if (data == NULL) {
        cap  = 32;
        data = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!data) abort();
        last = 0;
    }

    if (n > last) {
        ensure_capacity(n);
        for (idx_t k = last + 1; k <= n; ++k) {
            if (k <= 17) {
                data[k] = init_vals[k];
            } else {
                /* Compute the recurrence */
                seq_t v = data[k-17]
                          - 5ULL * data[k-16]
                          + 4ULL * data[k-15]
                          + 20ULL * data[k-14]
                          - 40ULL * data[k-13]
                          - 16ULL * data[k-12]
                          + 100ULL * data[k-11]
                          - 44ULL * data[k-10]
                          - 110ULL * data[k-9]
                          + 110ULL * data[k-8]
                          + 44ULL * data[k-7]
                          - 100ULL * data[k-6]
                          + 16ULL * data[k-5]
                          + 40ULL * data[k-4]
                          - 20ULL * data[k-3]
                          - 4ULL * data[k-2]
                          + 5ULL * data[k-1];
                data[k] = v;
            }
        }
        last = n;
    }
    return data[n];
}
