/*
 * A109787  Expansion of -(1-x-2*x^2+11*x^4-3*x^3) /
 *          ((x-1)*(x+1)*(x^2-3*x+1)*(1+x^2)).
 * https://oeis.org/A109787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) + a(n-4) - 3*a(n-5) + a(n-6)  for n>5
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=21, a(5)=58 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *memo = NULL;
    static idx_t  memo_size = 0;      /* Highest index + 1 that is already computed */
    static size_t memo_cap  = 0;      /* Allocated capacity (number of seq_t slots) */

    /* Ensure capacity for indices up to n (n+1 elements) */
    size_t needed_cap = (size_t)n + 1;
    if (needed_cap > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap : 16;
        while (new_cap < needed_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – return 0 as a safe value */
        memo   = tmp;
        memo_cap = new_cap;
    }

    /* Initialise first six values if not already done */
    if (memo_size < 6) {
        /* Make sure we have space for six entries */
        if (memo_cap < 6) {
            seq_t *tmp = (seq_t *)realloc(memo, 6 * sizeof(seq_t));
            if (!tmp) return 0;
            memo = tmp;
            memo_cap = 6;
        }
        memo[0] = 1;   /* a(0) */
        memo[1] = 2;   /* a(1) */
        memo[2] = 3;   /* a(2) */
        memo[3] = 4;   /* a(3) */
        memo[4] = 21;  /* a(4) */
        memo[5] = 58;  /* a(5) */
        memo_size = 6;
    }

    /* If we already have computed up to n, return the value */
    if (n < memo_size) {
        return memo[(size_t)n];
    }

    /* Compute values from memo_size up to n */
    for (idx_t i = memo_size; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 0: val = 1; break;
            case 1: val = 2; break;
            case 2: val = 3; break;
            case 3: val = 4; break;
            case 4: val = 21; break;
            case 5: val = 58; break;
            default: {
                /* recurrence: a(i) = 3*a(i-1) - a(i-2) + a(i-4) - 3*a(i-5) + a(i-6) */
                val = 3 * memo[(size_t)(i-1)]
                      - memo[(size_t)(i-2)]
                      + memo[(size_t)(i-4)]
                      - 3 * memo[(size_t)(i-5)]
                      + memo[(size_t)(i-6)];
            }
        }
        memo[(size_t)i] = val;
    }
    memo_size = n + 1;
    return memo[(size_t)n];
}
