/*
 * A162934  Shift sequence A162932 twice then subtract from the original
 *          sequence.
 * https://oeis.org/A162934
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^n in Σ_{k≥0} q^{3k+6} / ∏_{j=1..k}(1 -
 *            q^{j+2})
 * Range:     u8 n <= 49, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A162934 by dynamic programming over part sizes. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;   /* offset of the sequence */

    /* static cache */
    static seq_t *seq_arr = NULL;
    static seq_t *dp_arr  = NULL;
    static idx_t  alloc   = 0;     /* allocated array size */
    static idx_t  cur_max = 0;     /* highest index computed */

    if (n > cur_max)          /* need to recompute up to a larger n */
    {
        /* allocate new space if needed */
        if (n >= alloc)
        {
            seq_arr = (seq_t *)realloc(seq_arr, (n + 1) * sizeof(seq_t));
            dp_arr  = (seq_t *)realloc(dp_arr , (n + 1) * sizeof(seq_t));
            alloc = n + 1;
        }

        /* initialise arrays */
        for (idx_t i = 0; i <= n; ++i)
        {
            seq_arr[i] = (seq_t)0;
            dp_arr[i]  = (seq_t)0;
        }
        /* dp_arr represents partitions with part sizes 3..(k+2).  For k=0 no parts allowed. */
        dp_arr[0] = (seq_t)1;

        idx_t max_k_needed = (n - 6) / 3;   /* largest k that can contribute to a[n] */

        for (idx_t k = 0; k <= max_k_needed; ++k)
        {
            idx_t shift = 3 * k + 6;        /* minimal weight contributed by this k */

            /* add f_k's contribution to a[n] */
            for (idx_t i = shift; i <= n; ++i)
            {
                seq_arr[i] += dp_arr[i - shift];
            }

            /* update dp_arr to include part size s = k+3 (next part value) */
            idx_t s = k + 3;
            for (idx_t i = s; i <= n; ++i)
            {
                dp_arr[i] += dp_arr[i - s];
            }
        }

        cur_max = n;
    }

    return seq_arr[n];
}
