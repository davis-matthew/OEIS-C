/*
 * A210859  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210858; see the Formula section.
 * https://oeis.org/A210859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened triangle of
 *            coefficients of the polynomials v(n,x) defined by u(1,x)=1,
 *            v(1,x)=1 u(n,x)=u(n-1,x)+x*v(n-1,x)
 *            v(n,x)=(x+n)*u(n-1,x)+x*v(n-1,x) (n>1)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE:  The function computes the sequence lazily and memoises all
          calculated values.  No external dependencies are required. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of calculated terms (1‑indexed).  cache_cap is the
       current maximum allocated index.  computed_row is the highest row
       produced; computed_nmax = T(computed_row).  */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_row = 0;
    static idx_t computed_nmax = 0;            /* T(computed_row) */
    static seq_t *prev_u = NULL;
    static seq_t *prev_v = NULL;

    if (n == 0)            /* sequence is defined for n>=1 */
        return 0;

    /* If the requested n is already in the cache, return it. */
    if (n <= computed_nmax)
        return cache[n];

    /* Compute rows until we cover n.  The loop is incremental; we may
       be called with n in any order. */
    for (idx_t r = computed_row + 1; ; ++r)
    {
        /* Allocate arrays for the current row.  Length = r (coefficients
           for exponents 0..r-1). */
        idx_t cur_len = r;
        seq_t *cur_u = (seq_t *)malloc(cur_len * sizeof(seq_t));
        seq_t *cur_v = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur_u || !cur_v) return 0;  /* allocation failure */

        if (r == 1) {                        /* base row */
            cur_u[0] = 1;
            cur_v[0] = 1;
        } else {
            /* Length of the previous row */
            idx_t prev_len = r - 1;
            /* u(r,x) = u(r-1,x) + x * v(r-1,x) */
            for (idx_t k = 0; k < cur_len; ++k) {
                seq_t uprev = (k < prev_len) ? prev_u[k] : 0;
                seq_t vprev = (k > 0)      ? prev_v[k-1] : 0;
                cur_u[k] = uprev + vprev;
            }
            /* v(r,x) = (x + r) * u(r-1,x) + x * v(r-1,x)   (r > 1) */
            seq_t rn = (seq_t)r;                 /* cast to seq_t */
            cur_v[0] = rn * prev_u[0];          /* constant term */
            for (idx_t k = 1; k < cur_len; ++k) {
                seq_t term1 = (k < prev_len) ? rn * prev_u[k] : 0;
                seq_t term2 = prev_u[k-1];          /* previous u shifted */
                seq_t term3 = prev_v[k-1];          /* previous v shifted */
                cur_v[k] = term1 + term2 + term3;
            }
        }

        /* Store the v coefficients into the global cache.
           Position in cache (1‑indexed) = T(r-1) + k + 1.           */
        idx_t start_pos = ((r-1) * r) / 2 + 1; /* T(r-1)+1 */
        for (idx_t k = 0; k < cur_len; ++k) {
            idx_t pos = start_pos + k;          /* 1‑indexed position */
            if (pos > cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap < pos)
                    new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[pos] = cur_v[k];
        }

        /* Free the previous row (except the first).  The current row becomes
           the previous row for the next iteration. */
        free(prev_u);
        free(prev_v);
        prev_u = cur_u;
        prev_v = cur_v;

        /* Update state */
        computed_row = r;
        computed_nmax = ((idx_t)r * (r + 1)) / 2;

        /* Stop if we have computed enough terms. */
        if (n <= computed_nmax) {
            return cache[n];
        }
        /* Otherwise compute the next row. */
    }
}
