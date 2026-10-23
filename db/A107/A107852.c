/*
 * A107852  Expansion of -x*(x^2+1)*(x+1)^2/((2*x^3+x^2-1)*(x^4+1)).
 * https://oeis.org/A107852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - a(n-4) + a(n-6) + 2*a(n-7) for n > 6
 *            Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=7,
 *            a(6)=10
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial constants for indices 0..6 */
    static const seq_t init[7] = {0, 1, 2, 3, 6, 7, 10};

    /* static storage for memoisation */
    static seq_t *dp = NULL;
    static size_t dp_size = 0;      /* allocated size */
    static idx_t  computed = 0;     /* largest index already computed */

    /* allocate and initialise on first call */
    if (dp == NULL) {
        dp_size = 7;
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i) dp[i] = init[i];
        computed = 6;                /* values up to index 6 are known */
    }

    /* simple request for an already known value */
    if (n < dp_size) return dp[(size_t)n];

    /* enlarge array to hold up to index n */
    size_t new_size = (size_t)n + 1;
    if (new_size > dp_size) {
        dp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        dp_size = new_size;
    }

    /* compute missing values sequentially */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 6) {                 /* safety: keep initial entries */
            dp[(size_t)i] = init[i];
            continue;
        }
        seq_t val2 = dp[(size_t)(i - 3)];
        val2 = val2 + val2;           /* 2 * a(i-3) */
        seq_t val7 = dp[(size_t)(i - 7)];
        val7 = val7 + val7;           /* 2 * a(i-7) */

        seq_t result = dp[(size_t)(i - 2)]     /* a(i-2) */
                    + val2                      /* + 2*a(i-3) */
                    - dp[(size_t)(i - 4)]       /* - a(i-4) */
                    + dp[(size_t)(i - 6)]       /* + a(i-6) */
                    + val7;                    /* + 2*a(i-7) */

        dp[(size_t)i] = result;
    }

    computed = n;
    return dp[(size_t)n];
}
