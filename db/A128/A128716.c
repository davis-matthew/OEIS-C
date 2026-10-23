/*
 * A128716  Triangle where the n-th row, of n terms in order, contains
 *          consecutive multiples of n. The smallest term of row n is the
 *          smallest integer greater than or equal to the largest term of row
 *          (n-1), for n >= 2.
 * https://oeis.org/A128716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n,1) = 1 for n = 1;  for n ≥ 2,
   T(n,1) = ceil(T(n‑1,n‑1) / n) * n
   T(n,k+1) = T(n,k) + n  (for 1 ≤ k < n)
   The sequence a(n) is obtained by listing all T(n,k) in row‑major
   order (first row 1, then row 2, …).  a(1)=1, a(2)=2, a(3)=4, …   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache data */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t terms      = 0;   /* number of terms stored (1‑based) */
    static idx_t  last_row   = 0;   /* last row computed */
    static seq_t  prev_last  = 0;   /* last term of previous row */

    if (n == 0) return 0; /* offset is 1, so this case is undefined */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 128;                 /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache as needed */
    while (terms < n) {
        /* compute next row */
        idx_t  row = last_row + 1;
        seq_t  row_fac = (seq_t)row;
        seq_t  first;

        if (row == 1) {
            first = (seq_t)1;
        } else {
            /* ceil division: (prev_last + row_fac - 1) / row_fac */
            seq_t div = (prev_last + row_fac - (seq_t)1) / row_fac;
            first = div * row_fac;
        }

        /* generate all elements of this row */
        for (idx_t k = 0; k < row; ++k) {
            seq_t val = first + (seq_t)k * row_fac;

            if (terms >= cache_cap) {
                size_t newcap = cache_cap * 2;
                if (newcap < terms + 1) newcap = terms + 1;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                cache = tmp;
                cache_cap = newcap;
            }
            cache[terms] = val;
            ++terms;
        }

        /* update for next iteration */
        last_row = row;
        prev_last = first + (seq_t)(row - 1) * row_fac;
    }

    return cache[n-1];
}
