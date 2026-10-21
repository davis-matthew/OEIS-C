/*
 * A063117  Dimension of the space of weight 2n cusp forms for Gamma_0( 49 ).
 * https://oeis.org/A063117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=10, a(3)=20, a(4)=28, a(5)=38, a(n) = a(n-1) +
 *            a(n-3) - a(n-4) for n > 5
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;              /* dynamic array of computed terms */
static idx_t  memo_cap = 0;             /* current capacity of memo (in indices) */
static idx_t  memo_max_calculated = 0;  /* largest index with a value computed */

/* Compute a(n) – dimension of weight 2n cusp forms for Γ0(49) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used in sequence, but harmless */

    /* Ensure array has room for indices up to n */
    if (n + 1 > memo_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* On allocation failure return 0 – caller will treat as error */
            return 0;
        }
        /* Zero-initialize new slots to avoid indeterminate values */
        for (idx_t i = memo_cap; i < new_cap; ++i) tmp[i] = 0;
        memo = tmp;
        memo_cap = (idx_t)new_cap;
    }

    /* Handle the first five terms directly */
    if (n <= 5) {
        if (memo_cap < 6) { /* safety check, should already be satisfied */
            size_t new_cap = 6;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;
            for (idx_t i = memo_cap; i < new_cap; ++i) tmp[i] = 0;
            memo = tmp;
            memo_cap = (idx_t)new_cap;
        }
        if (memo_max_calculated < 5) {
            memo[1] = (seq_t)1;
            memo[2] = (seq_t)10;
            memo[3] = (seq_t)20;
            memo[4] = (seq_t)28;
            memo[5] = (seq_t)38;
            memo_max_calculated = 5;
        }
        return memo[n];
    }

    /* Now n > 5: compute missing values up to n */
    if (memo_max_calculated < 5) {
        /* initialize the first five terms */
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)10;
        memo[3] = (seq_t)20;
        memo[4] = (seq_t)28;
        memo[5] = (seq_t)38;
        memo_max_calculated = 5;
    }

    if (memo_max_calculated < n) {
        for (idx_t i = memo_max_calculated + 1; i <= n; ++i) {
            memo[i] = memo[i-1] + memo[i-3] - memo[i-4];
        }
        memo_max_calculated = n;
    }

    return memo[n];
}
