/*
 * A102620  Number of legal Go positions on a 1 X n board (for which 3^n is a
 *          trivial upper bound).
 * https://oeis.org/A102620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3), a(1)=1, a(2)=5, a(3)=15
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;   /* cached values, indexed by n, 1‑based */
static idx_t memo_size = 0;  /* current allocated size (elements) */
static idx_t memo_max = 0;   /* maximum n that has been computed */

/* Compute a(n) using the recurrence, with memoization */
seq_t a(idx_t n)
{
    if (n == 0)                     /* below offset; undefined, return 0 */
        return (seq_t)0;

    /* First call: allocate minimal storage */
    if (memo == NULL) {
        memo_size = 4;              /* indices 0..3; ignore index 0 */
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;  /* allocation failure */
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)5;
        memo[3] = (seq_t)15;
        memo_max = 3;
    }

    if (n <= memo_max)
        return memo[n];

    /* Grow the cache if necessary */
    if (n + 1 > memo_size) {
        idx_t new_size = memo_size;
        while (new_size <= n)
            new_size <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        memo = tmp;
        memo_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        if (i == 1)
            memo[1] = (seq_t)1;
        else if (i == 2)
            memo[2] = (seq_t)5;
        else if (i == 3)
            memo[3] = (seq_t)15;
        else
            memo[i] = (seq_t)3 * memo[i-1] - memo[i-2] + memo[i-3];
    }

    memo_max = n;
    return memo[n];
}
