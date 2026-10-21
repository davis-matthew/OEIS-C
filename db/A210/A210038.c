/*
 * A210038  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210037; see the Formula section.
 * https://oeis.org/A210038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the j‑th coefficient of the polynomial v(n,x+1) where
 *            the polynomials v(n,x) satisfy u(1,x)=v(1,x)=1
 *            u(n,x)=u(n-1,x)+v(n-1,x)+1 v(n,x)=u(n-1,x)+x*v(n-1,x)+1 The
 *            sequence lists these coefficients row by row, with row n
 *            containing the n coefficients of v(n,x+1). The implementation
 *            keeps the...
 * Range:     u8 n <= 29, u16 n <= 28, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* DO NOT CHANGE THE SIGNATURE */
seq_t a(idx_t n)
{
    /* Static cache of the flattened triangle */
    static seq_t *cache   = NULL;   /* array of coefficients */
    static size_t   cache_cap = 0;  /* capacity of cache */
    static size_t   cache_len = 0;  /* number of elements filled */

    /* Static storage of the previous row's u and v coefficients */
    static seq_t *u_prev = NULL;
    static seq_t *v_prev = NULL;
    static idx_t  prev_len = 0;    /* degree + 1 of the previous row */

    /* Ensure that we have at least n elements computed */
    while (cache_len < n)
    {
        /* Compute the next row (row index = max_row + 1) */
        idx_t new_row = prev_len;  /* because prev_len == current row - 1 */
        size_t len_new = (size_t)(new_row + 1); /* length of new row */

        /* Allocate arrays for the new u and v coefficients */
        seq_t *u_new = (seq_t *)calloc(len_new, sizeof(seq_t));
        seq_t *v_new = (seq_t *)calloc(len_new, sizeof(seq_t));
        if (!u_new || !v_new)
        {
            /* Allocation failure – free what we have and bail out */
            free(u_new);
            free(v_new);
            return (seq_t)0;
        }

        if (new_row == 0)
        {
            /* Base case: u(1,x)=v(1,x)=1 */
            u_new[0] = v_new[0] = (seq_t)1;
        }
        else
        {
            /* Recurrence: compute coefficients of u_new and v_new */
            for (size_t i = 0; i < len_new; ++i)
            {
                seq_t val_u = 0;
                seq_t val_v = 0;

                if (i < (size_t)prev_len)
                {
                    val_u += u_prev[i];
                    val_u += v_prev[i];
                    val_v += u_prev[i];
                }

                if (i > 0)
                {
                    val_v += v_prev[i - 1];
                }

                if (i == 0)
                {
                    val_u += (seq_t)1;
                    val_v += (seq_t)1;
                }

                u_new[i] = val_u;
                v_new[i] = val_v;
            }
        }

        /* Append coefficients of v_new(x+1) to the cache.
           For each degree i of v_new, its coefficient contributes
           to all lower degrees j via the binomial transform:
           w_j = Σ_{i=j}^{len_new-1} C(i,j) * v_new[i]        */
        size_t cache_needed = cache_len + len_new;
        if (cache_needed > cache_cap)
        {
            size_t new_cap = (cache_cap > 0) ? (cache_cap << 1) : 1;
            if (new_cap < cache_needed)
                new_cap = cache_needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
            {
                free(u_new);
                free(v_new);
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = 0; i < len_new; ++i)
        {
            seq_t bin = (seq_t)1; /* C(i,0) */
            for (size_t j = 0; j <= i; ++j)
            {
                cache[cache_len + j] += v_new[i] * bin;
                if (j < i)
                {
                    /* C(i,j+1) = C(i,j) * (i-j) / (j+1) */
                    bin = bin * (seq_t)(i - j) / (seq_t)(j + 1);
                }
            }
        }
        cache_len += len_new;

        /* Prepare for the next iteration */
        free(u_prev);
        free(v_prev);
        u_prev = u_new;
        v_prev = v_new;
        prev_len = (idx_t)len_new;
    }

    /* Return the requested entry (1‑based index) */
    return cache[n - 1];
}
