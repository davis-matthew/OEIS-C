/*
 * A202212  Triangle read by rows: T(n,k) (1 <= k <= n-1, n >= 2) =
 *          d(2*(n-k)-1)*(d(2*n-2)/d(2*(n-k)-2) - d(2*n-3)/d(2*(n-k)-3)),
 *          where d = A006882 is the double factorial function.
 * https://oeis.org/A202212
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows * T(n,k) = d(2*(n-k)-1) * * (
 *            d(2*n-2)/d(2*(n-k)-2) - d(2*n-3)/d(2*(n-k)-3) ), * with d =
 *            double factorial (OEIS A006882). * a(n) is the n‑th term (n ≥ 2)
 *            of the triangle row‐by‑row reading. * The offset is 2, so a(2)
 *            is the first term
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                 /* index below offset is irrelevant */

    /* static cache of computed terms (a(2)...a(current_max)) */
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;             /* allocated capacity     */
    static size_t cache_len    = 0;             /* number of terms stored */
    static idx_t  current_row  = 2;             /* next row to generate */
    static seq_t  P_prev       = (seq_t)1;      /* d(2*current_row-3), start with d(1)=1 */

    /* grow the cache while we still need more terms */
    while (cache_len < n - 1) {
        seq_t  P_local   = P_prev;          /* initial d(2*current_row-3)   */
        seq_t  ratio_even = (seq_t)2 * (seq_t)(current_row - 1);
        seq_t  ratio_odd  = ratio_even - (seq_t)1;

        for (idx_t k = 1; k < current_row; ++k) {
            seq_t diff = ratio_even - ratio_odd;
            seq_t term = P_local * diff;

            /* ensure cache has space for the new term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                while (new_cap <= cache_len) new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* unrecoverable allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = term;

            /* update for the next k (if any) */
            if (k < current_row - 1) {
                idx_t offset = current_row - k - 1;
                seq_t factor_even = (seq_t)2 * (seq_t)offset;          /* 2*(n-k-1)   */
                seq_t factor_odd  = factor_even - (seq_t)1;            /* 2*(n-k-1)-1 */
                ratio_even *= factor_even;
                ratio_odd  *= factor_odd;

                seq_t divisor = (seq_t)2 * (seq_t)(current_row - k) - (seq_t)1; /* 2*(n-k)-1 */
                P_local /= divisor;
            }
        }

        /* prepare the next row */
        ++current_row;
        seq_t factor_next = (seq_t)2 * (seq_t)current_row - (seq_t)3;   /* 2*next_n - 3   */
        P_prev *= factor_next;
    }

    return cache[n - 2];
}
