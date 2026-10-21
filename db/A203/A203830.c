/*
 * A203830  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1125 , a(2)=17289 , a(3)=270333 a(n) = 19*a(n-1) -
 *            54*a(n-2) + 32*a(n-3) (n >= 4) Computed iteratively; no static
 *            memoization
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0 or negative */

    /* Base cases */
    if (n == 1) return 1125;
    if (n == 2) return 17289;
    if (n == 3) return 270333;

    /* Allocate space for terms up to n */
    seq_t *term = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!term) abort();           /* allocation failure */

    /* Initialise first three terms */
    term[1] = 1125;
    term[2] = 17289;
    term[3] = 270333;

    /* Iterate using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = term[i - 1] * (seq_t)19;   /* 19*a(i-1) */
        seq_t t2 = term[i - 2] * (seq_t)54;   /* 54*a(i-2) */
        seq_t t3 = term[i - 3] * (seq_t)32;   /* 32*a(i-3) */
        term[i] = t1 - t2 + t3;
    }

    seq_t result = term[n];
    free(term);
    return result;
}
