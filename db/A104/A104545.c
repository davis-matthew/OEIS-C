/*
 * A104545  Number of Motzkin paths of length n having no consecutive (1,0)
 *          steps.
 * https://oeis.org/A104545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Motzkin paths of length n having no consecutive
 *            (1,0) steps. Computed via dynamic programming with states
 *            (height, last step was horizontal or not)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Handle the trivial case immediately */
    if (n == 0) return (seq_t)1;

    /* Static state to keep memoisation across calls */
    static idx_t memo_size = 0;            /* allocated capacity for memo array      */
    static idx_t computed = 0;             /* highest n that has been computed so far */
    static seq_t *memo = NULL;             /* memoisation array a[0..]              */
    static seq_t *dp0[2] = {NULL, NULL}, *dp1[2] = {NULL, NULL}; /* DP arrays */
    static int cur = 0, next = 1;          /* indices of current/non‑current arrays */

    /* Ensure enough capacity for the needed n */
    {
        idx_t needed = n + 1;
        if (needed > memo_size) {
            /* grow capacity to next power of two */
            idx_t new_cap = memo_size ? memo_size << 1 : 1;
            while (new_cap < needed) new_cap <<= 1;

            memo = (seq_t *)realloc((void *)memo, new_cap * sizeof(seq_t));
            dp0[0] = (seq_t *)realloc((void *)dp0[0], new_cap * sizeof(seq_t));
            dp0[1] = (seq_t *)realloc((void *)dp0[1], new_cap * sizeof(seq_t));
            dp1[0] = (seq_t *)realloc((void *)dp1[0], new_cap * sizeof(seq_t));
            dp1[1] = (seq_t *)realloc((void *)dp1[1], new_cap * sizeof(seq_t));
            /* zero the new parts of each array */
            memset((void *)(memo + memo_size), 0,
                   (new_cap - memo_size) * sizeof(seq_t));
            memset((void *)(dp0[0] + memo_size), 0,
                   (new_cap - memo_size) * sizeof(seq_t));
            memset((void *)(dp0[1] + memo_size), 0,
                   (new_cap - memo_size) * sizeof(seq_t));
            memset((void *)(dp1[0] + memo_size), 0,
                   (new_cap - memo_size) * sizeof(seq_t));
            memset((void *)(dp1[1] + memo_size), 0,
                   (new_cap - memo_size) * sizeof(seq_t));
            memo_size = new_cap;
        }
    }

    /* If already computed up to n, return memoised value */
    if (computed >= n) return memo[n];

    /* Initialise base state if this is the first computation */
    if (computed == 0) {
        /* All arrays are already zeroed by realloc/alloc */
        dp0[cur][0] = (seq_t)1;   /* start at height 0, last step NOT horizontal */
        /* dp1[cur][0] is already 0 */
        memo[0] = (seq_t)1;       /* a(0) = 1 */
    }

    /* Compute all terms from computed+1 up to n */
    for (idx_t step = computed + 1; step <= n; ++step) {
        /* Zero the next DP arrays for indices up to 'step' */
        for (idx_t i = 0; i <= step; ++i) {
            dp0[next][i] = 0;
            dp1[next][i] = 0;
        }

        /* Perform transitions for all reachable heights */
        for (idx_t h = 0; h < step; ++h) {
            seq_t val0 = dp0[cur][h];
            if (val0) {
                /* From a state where the last step was NOT horizontal */
                dp0[next][h + 1] += val0;      /* Up */
                if (h > 0) dp0[next][h - 1] += val0;   /* Down */
                dp1[next][h] += val0;          /* Horizontal */
            }
            seq_t val1 = dp1[cur][h];
            if (val1) {
                /* From a state where the last step WAS horizontal (no further horizontal) */
                dp0[next][h + 1] += val1;      /* Up */
                if (h > 0) dp0[next][h - 1] += val1;   /* Down */
                /* Horizontal not allowed */
            }
        }

        /* Total paths ending at height 0 after this step */
        memo[step] = dp0[next][0] + dp1[next][0];

        /* Prepare for next iteration: swap current/next indices */
        int tmp = cur;
        cur = next;
        next = tmp;
    }

    computed = n;                 /* update the largest computed index */
    return memo[n];
}
