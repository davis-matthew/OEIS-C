/*
 * A189924  a(n) = abs(Stirling1(n+2,2)) - abs(Stirling1(n+2,3)).
 * https://oeis.org/A189924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = abs(Stirling1(n+2,2)) - abs(Stirling1(n+2,3))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *c2 = NULL;   /* abs(Stirling1(m,2)) for m up to current max */
static seq_t *c3 = NULL;   /* abs(Stirling1(m,3)) for m up to current max */
static idx_t cap = 0;      /* current capacity of the arrays */
static idx_t max_seen = 1; /* largest m already computed (m = n+2) */
static int initialized = 0;
static seq_t prev_v[4] = {0, 1, 0, 0};  /* previous row items for k=1..3 */

/* Compute a(n) as defined in the OEIS entry */
seq_t a(idx_t n)
{
    /* --- one‑time initialization --- */
    if (!initialized)
    {
        initialized = 1;
        /* allocate space for indices 0 and 1 */
        cap = 2;
        c2 = (seq_t *)malloc(cap * sizeof(seq_t));
        c3 = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!c2 || !c3) return 0;   /* out of memory – unlikely in tests */
        c2[0] = 0;  c3[0] = 0;
        c2[1] = 0;  c3[1] = 0;
        /* prev_v already contains the correct start values for k=1..3 */
    }

    idx_t m = n + 2;   /* we need Stirling1(m,2) and Stirling1(m,3) */

    /* Expand storage if required */
    if (m >= cap)
    {
        idx_t new_cap = m + 1;
        seq_t *tmp2 = (seq_t *)realloc(c2, new_cap * sizeof(seq_t));
        seq_t *tmp3 = (seq_t *)realloc(c3, new_cap * sizeof(seq_t));
        if (!tmp2 || !tmp3) return 0;   /* allocation failed */
        c2 = tmp2;
        c3 = tmp3;
        for (idx_t i = cap; i < new_cap; ++i)
        {
            c2[i] = 0;
            c3[i] = 0;
        }
        cap = new_cap;
    }

    /* Compute any missing rows up to m */
    if (m > max_seen)
    {
        for (idx_t i = max_seen + 1; i <= m; ++i)
        {
            seq_t mult  = (seq_t)(i - 1);
            seq_t curr1 = mult * prev_v[1];
            seq_t curr2 = prev_v[1] + mult * prev_v[2];
            seq_t curr3 = prev_v[2] + mult * prev_v[3];

            c2[i] = curr2;         /* abs(Stirling1(i,2)) */
            c3[i] = curr3;         /* abs(Stirling1(i,3)) */

            /* shift to next row */
            prev_v[1] = curr1;
            prev_v[2] = curr2;
            prev_v[3] = curr3;
        }
        max_seen = m;
    }

    /* The required value may be negative */
    return c2[m] - c3[m];
}
