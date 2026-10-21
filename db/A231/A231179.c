/*
 * A231179  Boustrophedon transform of nonnegative integers, cf. A001477.
 * https://oeis.org/A231179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Boustrophedon transform of the nonnegative integers (row
 *            n of the Boustrophedon triangle); a(0)=0, a(1)=1, a(2)=4,
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple iterative construction of the Boustrophedon triangle.
   The routine is memoised: each a(n) is stored once and reused in
   subsequent calls, independent of the order of the n arguments. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised sequence values */
static size_t cache_cap = 0;
static idx_t  max_done = 0;   /* highest n that has been computed */

static seq_t *prev_row = NULL;  /* previous triangular row */
static size_t prev_len = 0;      /* length of prev_row */

static void init(void)
{
    if (cache == NULL)
    {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed */
        cache[0] = (seq_t)0;                 /* a(0) */

        max_done = 0;

        prev_len = 1;
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        if (prev_row == NULL) abort();
        prev_row[0] = (seq_t)0;              /* row 0 */
    }
}

seq_t a(idx_t n)
{
    init();                       /* ensure tables exist */

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = (size_t)n + 1;
    }

    /* Compute new rows up to n */
    while (max_done < n)
    {
        ++max_done;
        idx_t i = max_done;                 /* current row index (1‑based) */

        seq_t *curr_row = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        if (curr_row == NULL) abort();

        seq_t ai = (seq_t)i;                /* the value s(i) = i */

        if (i & 1)                         /* odd row – left‑to‑right */
        {
            curr_row[0] = ai;
            for (idx_t k = 1; k <= i; ++k)
                curr_row[k] = curr_row[k - 1] + prev_row[k - 1];
            cache[i] = curr_row[i];        /* a(i) is last element */
        }
        else                                 /* even row – right‑to‑left */
        {
            curr_row[i] = ai;
            for (idx_t k = i - 1; ; --k)
            {
                curr_row[k] = curr_row[k + 1] + prev_row[k];
                if (k == 0) break;
            }
            cache[i] = curr_row[0];        /* a(i) is first element */
        }

        free(prev_row);
        prev_row = curr_row;
        prev_len = i + 1;
    }

    return cache[n];
}
