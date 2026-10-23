/*
 * A170633  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170633 – reduced words in the Coxeter group
 *
 *  The generating function is
 *      (t^48 + 2 t^47 + 2 t^46 + … + 2 t + 1)
 *      --------------------------------------------------
 *      1081 t^48 - 46 t^47 - … - 46 t + 1
 *
 *  Thus the coefficients satisfy the linear recurrence
 *
 *      a(0) = 1
 *
 *      For 1 ≤ n ≤ 47
 *          a(n) = 2 + 46 * Σ_{k=1}^{n} a(n-k)
 *
 *      For n = 48
 *          a(48) = 1 + 46 * Σ_{k=1}^{47} a(48-k) - 1081 * a(0)
 *
 *      For n > 48
 *          a(n) = 46 * Σ_{k=1}^{47} a(n-k) - 1081 * a(n-48)
 *
 *  The implementation memoises the sequence and expands it on
 *  demand.  It works for any seq_t type supplied by the build.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;          /* storage for a[0..]                     */
    static size_t buf_cap = 0;          /* current capacity of buf                */
    static size_t last = 0;             /* number of values computed (next free) */

    /* enlarge the buffer if necessary */
    if (buf_cap <= n) {
        size_t new_cap = (n < 64) ? 64 : (n * 2 + 16);
        buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        buf_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = last; i <= n; ++i) {
        seq_t rhs = 0;
        if (i == 0)           rhs = 1;
        else if (i <= 47)     rhs = 2;
        else if (i == 48)     rhs = 1;

        seq_t sum = 0;
        /* add 46 * a(i-j) for j = 1..min(i,47) */
        for (idx_t j = 1; j <= i && j <= 47; ++j)
            sum += (seq_t)46 * buf[i - j];

        /* subtract 1081 * a(i-48) if i ≥ 48 */
        if (i >= 48)
            sum -= (seq_t)1081 * buf[i - 48];

        buf[i] = rhs + sum;
    }
    last = n + 1;  /* next free position */

    return buf[n];
}
