/*
 * A210159  Number of (n+1)X3 0..3 arrays with every 2X2 subblock having two
 *          or three distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A210159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210159:  Number of (n+1)×3 0..3 arrays with every 2×2 subblock having two or three distinct values,
   new values 0..3 introduced in row major order.
   Empirical recurrence (valid for n≥8):
       a(n) = 55*a(n-1) - 186*a(n-2) - 1230*a(n-3) + 2723*a(n-4)
              + 159*a(n-5) - 1604*a(n-6) + 468*a(n-7)
   Initial values (offset 1):
       a(1)=148, a(2)=6977, a(3)=351482, a(4)=17864271,
       a(5)=908980864, a(6)=46257884813, a(7)=2354098130302
*/

#include <stdlib.h>
/* seq_t and idx_t are provided by the build system */

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *memo = NULL;      /* array of values, 1‑based */
    static size_t memo_cap = 0;     /* allocated capacity (elements) */
    static size_t memo_len = 0;     /* highest index computed */

    /* helper: grow the array to at least "needed" elements (1‑based) */
    if (memo_cap < (size_t)n + 1) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 16;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE); /* fatal allocation error */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* initialise the first seven terms once (if not already done) */
    if (memo_len == 0) {
        memo[1] = 148;
        memo[2] = 6977;
        memo[3] = 351482;
        memo[4] = 17864271;
        memo[5] = 908980864;
        memo[6] = 46257884813;
        memo[7] = 2354098130302;
        memo_len = 7;
    }

    /* compute new values up to requested n */
    for (size_t i = memo_len + 1; i <= (size_t)n; ++i) {
        seq_t val = 55 * memo[i-1] - 186 * memo[i-2]
                  - 1230 * memo[i-3] + 2723 * memo[i-4]
                  + 159 * memo[i-5] - 1604 * memo[i-6]
                  + 468 * memo[i-7];
        memo[i] = val;
    }

    /* keep the maximum index computed */
    if ((size_t)n > memo_len)
        memo_len = (size_t)n;

    return memo[(size_t)n];
}
