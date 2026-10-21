/*
 * A206067  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three clockwise edge increases.
 * https://oeis.org/A206067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 56*a(n-2) + 82*a(n-3) - 47*a(n-4)
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (empirical recurrence valid for n > 9)                           */

#include <stdlib.h>

/* The function uses a simple linear recurrence with memoization.
   Initial values (for n = 1 … 9) are taken from the OEIS entry.  */
seq_t a(idx_t n)
{
    /* Initial values up to a(9).  Index 0 is unused. */
    static const seq_t init_vals[10] = {
        0ULL,
        (seq_t)537,
        (seq_t)3330,
        (seq_t)27654,
        (seq_t)233058,
        (seq_t)1967814,
        (seq_t)16608978,
        (seq_t)140136942,
        (seq_t)1182218562,
        (seq_t)9972840630
    };

    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    /* Memoization tables.  They persist across calls. */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;   /* allocated size */
    static idx_t cached_n = 0;    /* last computed index */

    /* Initial allocation on first call. */
    if (memo == NULL) {
        memo_size = 10;                /* up to index 9 inclusive */
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_size);
        for (idx_t i = 0; i < memo_size; ++i)
            memo[i] = init_vals[i];
        cached_n = 9;                  /* we have values 1..9 */
    }

    /* Return cached value if already computed. */
    if (n <= cached_n)
        return memo[n];

    /* Ensure buffer is large enough. */
    if (n >= memo_size) {
        idx_t new_size = memo_size * 2;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * new_size);
        if (!tmp) { /* allocation failure – fall back to last value */
            return memo[cached_n];
        }
        memo = tmp;
        memo_size = new_size;
    }

    /* Compute values from cached_n+1 up to n using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t t1 = 14 * memo[i-1];
        t1 += 82 * memo[i-3];
        seq_t t2 = 56 * memo[i-2];
        t2 += 47 * memo[i-4];
        memo[i] = t1 - t2;   /* result is guaranteed non‑negative */
    }

    cached_n = n;
    return memo[n];
}
