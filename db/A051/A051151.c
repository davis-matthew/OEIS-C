/*
 * A051151  Triangle read by rows: T(n, m) = S1(n, m)*6^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051151 – a(n) = T(r,m) where T(1,1)=1,
   T(n,1)=−6·(n−1)·T(n−1,1),  T(n,n)=1,
   T(n,m)=T(n−1,m−1)−6·(n−1)·T(n−1,m)  (2≤m≤n−1).
   The sequence is the triangle read row‑wise. */

/* In the build this file is seen as C11 /\ C++17 code.
   seq_t and idx_t are supplied by the build environment.
   No other headers beyond <stdlib.h> and <stdint.h> are required. */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*   Static state for memoisation                                    */
/* ------------------------------------------------------------------ */
static seq_t *seq      = NULL;    /* flattened sequence values           */
static size_t  seq_cap = 0;       /* capacity of seq                    */
static idx_t   seq_len = 0;       /* how many values have been computed */

static idx_t   computed_row     = 0; /* last fully processed row (1‑based) */
static seq_t *prev_row = NULL;       /* array holding T(computed_row,*)  */
static size_t prev_row_size = 0;     /* its length (equal to computed_row) */

/* ------------------------------------------------------------------ */
/*   Ensure that seq[0 .. n-1] are available                         */
/* ------------------------------------------------------------------ */
static void ensure_up_to(idx_t n)
{
    /* nothing to do if already ready */
    if (seq_len >= n)
        return;

    /* iterate rows until we have enough terms */
    while (seq_len < n)
    {
        /* next row to compute */
        idx_t r = computed_row + 1;
        size_t r_size = (size_t)r;

        /* allocate array for current row */
        seq_t *curr = (seq_t *)malloc(r_size * sizeof(seq_t));
        if (!curr)
            return; // allocation failure – leave incomplete

        /* compute the current row */
        if (r == 1)
        {
            /* base case: T(1,1) = 1 */
            curr[0] = (seq_t)1;
        }
        else
        {
            seq_t mult = (seq_t)(6 * (r - 1));        /* 6*(n-1) */
            /* first element: T(r,1) = −mult * T(r-1,1) */
            curr[0] = -(mult * prev_row[0]);

            /* middle elements (if any) */
            for (idx_t m = 2; m < r; ++m)
            {
                /* prev_row has length r-1, indices m-2 and m-1 exist */
                curr[m - 1] = prev_row[m - 2] - (mult * prev_row[m - 1]);
            }

            /* diagonal element: T(r,r) = 1 */
            curr[r - 1] = (seq_t)1;
        }

        /* enlarge the flattened array if necessary */
        size_t needed = seq_len + r_size;
        if (needed > seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp)
            {
                free(curr);
                return; /* allocation failure */
            }
            seq = tmp;
            seq_cap = new_cap;
        }

        /* copy current row into the flattened array */
        for (size_t i = 0; i < r_size; ++i)
            seq[seq_len + i] = curr[i];

        seq_len += r_size;                 /* update terms count        */
        free(prev_row);                    /* drop old row              */
        prev_row = curr;                   /* keep current row for next  */
        prev_row_size = r_size;
        computed_row = r;                  /* remember last row processed */
    }
}

/* ------------------------------------------------------------------ */
/*   Public API                                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* per requirement, off‑offset */

    ensure_up_to(n);                      /* guarantees seq[n-1] ready */

    return seq[n - 1];
}
