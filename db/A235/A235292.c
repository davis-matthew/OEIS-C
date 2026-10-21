/*
 * A235292  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = 3*a(n-1)+17*a(n-2)-57*a(n-3)-86*a(n-4)+372*a(n-5)
 *                    +76*a(n-6)-972*a(n-7)+360*a(n-8)+864*a(n-9)-576*a(n-10)
 *                for n ≥ 11; initial terms:
 *                a(1)=108, a(2)=232, a(3)=476, a(4)=1132, a(5)=2556,
 *                a(6)=6532, a(7)=15692, a(8)=42100, a(9)=104988, a(10)=290932
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoization table (index 0 unused) */
    static seq_t *table = NULL;
    static idx_t  cap   = 0;   /* allocated size */
    static idx_t  used  = 0;   /* highest index filled */

    /* known first 10 terms (1‑based) */
    static const seq_t init[10] = {
        108, 232, 476, 1132, 2556,
        6532, 15692, 42100, 104988, 290932
    };

    if (n <= 10) {
        /* direct lookup for the initial terms */
        return init[n - 1];
    }

    /* enlarge table if necessary */
    if (n > used) {
        idx_t new_cap = n + 1;          /* reserve indices 0..n */
        seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        if (!tmp) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* allocation failed */
        }
        table = tmp;
        cap   = new_cap;

        /* initialise the first ten entries if they are not already present */
        if (used < 10) {
            for (idx_t i = used + 1; i <= 10; ++i) {
                table[i] = init[i - 1];
            }
            used = 10;
        }

        /* compute terms up to n (inclusive) */
        for (idx_t i = used + 1; i <= n; ++i) {
            seq_t val =
                table[i - 1] * 3
              + table[i - 2] * 17
              - table[i - 3] * 57
              - table[i - 4] * 86
              + table[i - 5] * 372
              + table[i - 6] * 76
              - table[i - 7] * 972
              + table[i - 8] * 360
              + table[i - 9] * 864
              - table[i -10] * 576;
            table[i] = val;
        }
        used = n;
    }
    return table[n];
}
