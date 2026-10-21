/*
 * A229535  Number of defective 3-colorings of a 2 X n 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly one
 *          mistake, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4) (n ≥ 6) Initial values:
 *            a(1)=0, a(2)=4, a(3)=8, a(4)=36, a(5)=112
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization buffer */
    static seq_t *memo   = NULL;  /* array of values 0..cur */
    static size_t cap    = 0;     /* allocated capacity (in seq_t elements) */
    static idx_t  cur    = 0;     /* highest index computed so far */

    /* handle out‑of‑range (optional, not required by OEIS) */
    if (n == 0) return (seq_t)0;

    /* first call – allocate and initialize base values */
    if (memo == NULL) {
        /* need at least indices 0..5 (a(1) to a(5)) */
        size_t need = (size_t)n + 1;
        if (need < 6) need = 6;

        memo = (seq_t *)calloc(need, sizeof(seq_t));
        if (memo == NULL) abort();          /* allocation failure */

        /* base cases */
        memo[0] = (seq_t)0;   /* unused, but safe */
        memo[1] = (seq_t)0;   /* a(1) */
        memo[2] = (seq_t)4;   /* a(2) */
        memo[3] = (seq_t)8;   /* a(3) */
        memo[4] = (seq_t)36;  /* a(4) */
        memo[5] = (seq_t)112; /* a(5) */

        cap = need;
        cur = 5;
    }

    /* if the caller wants a bigger n, grow the buffer */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation failure */
        memo = tmp;
        cap  = new_cap;
    }

    /* compute any missing terms up to n (recurrence holds for n >= 6) */
    if (n > cur) {
        for (idx_t i = cur + 1; i <= n; ++i) {
            memo[i] = ( seq_t)(4 * memo[i - 1]
                               - 8 * memo[i - 3]
                               - 4 * memo[i - 4] );
        }
        cur = n;
    }

    return memo[n];
}
