/*
 * A091187  Triangle read by rows: T(n,k) is the number of ordered trees with
 *          n edges and k branches.
 * https://oeis.org/A091187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r,c are the row and column obtained from the
 *            flattened index n. T(r,c) = M(c-1) * binomial(r-1, c-1) with M
 *            the Motzkin numbers (M(0)=1, M(1)=1,
 *            M(n)=((2n+1)M(n-1)+(3n-3)M(n-2))/(n+2))
 * Range:     u8 n <= 27, u16 n <= 85, u32 n <= 224, u64 n <= 729,
 *            u128 n <= 2534, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n_flat)
{
    if (n_flat < 1)
        return (seq_t)0;   /* undefined for 0, but return 0 */

    /* Find row r such that r*(r+1)/2 >= n_flat */
    double d = sqrt((double)(8.0 * n_flat - 7.0));
    idx_t r = (idx_t)((d + 1.0) / 2.0);
    /* Correct possible rounding errors */
    while (r * (r + 1) / 2 < n_flat) r++;
    while (r > 0 && (r - 1) * r / 2 >= n_flat) r--;

    /* Column index c (1 <= c <= r) */
    idx_t prev = r * (r - 1) / 2;
    idx_t c = n_flat - prev;                /* 1‑based column */

    /* ---------- compute Motzkin number M(c-1) with caching ---------- */
    static seq_t *M = NULL;                  /* array of Motzkin numbers */
    static size_t M_size = 0;                /* allocated size          */
    static idx_t M_max = 0;                  /* highest index computed  */

    idx_t need = c - 1;                      /* we need M(need)         */
    if (M == NULL) {
        M_size = 2;                          /* hold M(0) and M(1)     */
        M = (seq_t *)malloc(M_size * sizeof(seq_t));
        M[0] = (seq_t)1;                     /* M(0) */
        M[1] = (seq_t)1;                     /* M(1) */
        M_max = 1;
    }

    while (M_max < need) {                   /* extend the table          */
        idx_t n = M_max + 1;                 /* new index to compute     */
        seq_t m_prev1 = M[n - 1];
        seq_t m_prev2 = M[n - 2];
        seq_t numerator = ((seq_t)(2 * n + 1)) * m_prev1
                        + ((seq_t)(3 * n - 3)) * m_prev2;
        seq_t m_new = numerator / ((seq_t)(n + 2));
        if (M_size <= (size_t)n) {           /* enlarge if necessary     */
            size_t new_size = M_size * 2;
            if (new_size <= (size_t)n) new_size = n + 1;
            seq_t *tmp = (seq_t *)realloc(M, new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0;       /* allocation failure       */
            M = tmp;
            M_size = new_size;
        }
        M[n] = m_new;
        M_max = n;
    }
    seq_t motz = M[need];                    /* M(c-1)                    */

    /* ---------- compute binomial coefficient ---------- */
    seq_t bin = binom(r - 1, c - 1);

    /* ---------- final result ---------- */
    return motz * bin;
}
