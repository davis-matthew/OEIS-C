/*
 * A121584  Number of cells in columns 1 and 2 of all deco polyominoes of
 *          height n. A deco polyomino is a directed column-convex polyomino
 *          in which the height, measured along the diagonal, is attained only
 *          in the last column.
 * https://oeis.org/A121584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121584 - Number of cells in columns 1 and 2 of all deco polyominoes of height n
   Recurrence (n ≥ 3):
       a(1) = 1
       a(2) = 4
       a(n) = ((2n-3)a(n-1)-(n-1)a(n-2))/(n-2)
               + ((2n^3-9n^2+17n-16)/2) * ((n-1)!/(n-2))
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t  cap   = 0;      /* allocated size of cache (index 0 unused) */
    static idx_t   last  = 0;      /* highest index computed so far */
    static seq_t   fact  = 0;      /* factorial of (last-2)! ready for next step */

    /* direct answers for the basis */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* Ensure cache large enough */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* First time initialization */
    if (last == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        last     = 2;
        fact     = (seq_t)1; /* factorial of (last-2)! = 0! = 1, ready for i=3 */
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Update factorial: fact now equals (i-3)!  (i > 3 only multiplies) */
        if (i > 3)
            fact *= (seq_t)(i - 3);

        /* Term 1: ((2i-3)a(i-1)-(i-1)a(i-2))/(i-2) */
        seq_t num1 = ((seq_t)(2 * i - 3)) * cache[i - 1];
        seq_t num2 = ((seq_t)(i - 1)) * cache[i - 2];
        seq_t term1 = (num1 - num2) / (seq_t)(i - 2);

        /* Term 2: ((2i^3-9i^2+17i-16)/2) * ((i-1)!/(i-2)) */
        seq_t i_seq = (seq_t)i;
        seq_t poly = 2 * i_seq * i_seq * i_seq 
                    - 9 * i_seq * i_seq 
                    + 17 * i_seq 
                    - 16;
        seq_t poly_div2 = poly / 2;                     /* always integral */
        seq_t fac_ratio = (i_seq - 1) * fact;           /* (i-1)!/(i-2)  */
        seq_t term2 = poly_div2 * fac_ratio;

        /* Store result */
        cache[i] = term1 + term2;
    }

    last = n;
    return cache[n];
}
