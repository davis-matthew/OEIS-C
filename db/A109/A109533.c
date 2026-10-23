/*
 * A109533  a(n) = a(n-3) + 2*a(n-6) + a(n-9).
 * https://oeis.org/A109533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109533: a(n) = a(n-3) + 2*a(n-6) + a(n-9)
   with initial values a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=1,
   a(5)=3, a(6)=5, a(7)=1, a(8)=6.  */
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* external linkage as requested */
seq_t a(idx_t n) {
#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic construction */
    static std::vector<seq_t> dp;
    /* initialise base values once */
    if (dp.size() < 9) {
        dp.resize(9);
        dp[0] = (seq_t)1; dp[1] = (seq_t)0; dp[2] = (seq_t)1;
        dp[3] = (seq_t)2; dp[4] = (seq_t)1; dp[5] = (seq_t)3;
        dp[6] = (seq_t)5; dp[7] = (seq_t)1; dp[8] = (seq_t)6;
    }
    /* return stored value if already computed */
    if (n < dp.size())
        return dp[(size_t)n];

    /* extend table up to n */
    size_t prev = dp.size();
    dp.resize((size_t)n + 1);
    for (idx_t i = (idx_t)prev; i <= n; ++i) {
        if (i >= 9) {
            dp[(size_t)i] = dp[(size_t)(i-3)] +
                            dp[(size_t)(i-6)] * (seq_t)2 +
                            dp[(size_t)(i-9)];
        }
        /* i < 9 already have base values */
    }
    return dp[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* initialise base values once */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * 9);
        dp_size = 9;
        dp[0] = (seq_t)1; dp[1] = (seq_t)0; dp[2] = (seq_t)1;
        dp[3] = (seq_t)2; dp[4] = (seq_t)1; dp[5] = (seq_t)3;
        dp[6] = (seq_t)5; dp[7] = (seq_t)1; dp[8] = (seq_t)6;
    }

    if (n < dp_size)
        return dp[(size_t)n];

    idx_t prev = dp_size;
    dp_size = n + 1;
    dp = (seq_t *)realloc(dp, sizeof(seq_t) * dp_size);

    for (idx_t i = prev; i <= n; ++i) {
        if (i >= 9) {
            dp[(size_t)i] = dp[(size_t)(i-3)] +
                            dp[(size_t)(i-6)] * (seq_t)2 +
                            dp[(size_t)(i-9)];
        }
        /* i < 9 already filled by base values */
    }
    return dp[(size_t)n];
#endif
}
