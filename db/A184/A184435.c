/*
 * A184435  Number of strings of numbers x(i=1..n) in 0..3 with sum i*x(i)^2
 *          equal to n*9.
 * https://oeis.org/A184435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sequences x[1..n] in 0..3 such that *
 *            Σ_{i=1..n} i * x[i]^2 = 9 * n * This is computed by a 1‑D DP
 *            over the weighted sum
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 29, u64 n <= 78, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, but safe guard */

    /* The required weighted sum */
    idx_t target = 9 * n;
    size_t sz = (size_t)target + 1;    /* size of dp arrays */

#if defined(__cplusplus)
    /* C++: use vector to default‑construct big integers */
    std::vector<seq_t> dp(sz, seq_t(0));
    std::vector<seq_t> next(sz, seq_t(0));
    seq_t *dp_ptr = dp.data();
    seq_t *next_ptr = next.data();
#else
    /* C: three‑byte zeroed array via calloc */
    seq_t *dp_ptr = calloc(sz, sizeof(seq_t));
    seq_t *next_ptr = calloc(sz, sizeof(seq_t));
#endif

    /* Initial condition: zero sum is achievable in exactly one way */
    dp_ptr[0] = 1;

    /* Iterate over each position i = 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t w1 = i;            /* weight for x[i] = 1  (1^2)   */
        idx_t w2 = 4 * i;        /* weight for x[i] = 2  (2^2)   */
        idx_t w3 = 9 * i;        /* weight for x[i] = 3  (3^2)   */

        /* Compute next DP layer */
        for (size_t t = 0; t < sz; ++t) {
            seq_t val = dp_ptr[t];                 /* y = 0, weight 0   */
            if (t >= w1) val += dp_ptr[t - w1];    /* y = 1             */
            if (t >= w2) val += dp_ptr[t - w2];    /* y = 2             */
            if (t >= w3) val += dp_ptr[t - w3];    /* y = 3             */
            next_ptr[t] = val;
        }

        /* Swap pointers for next iteration */
        seq_t *tmp = dp_ptr;
        dp_ptr = next_ptr;
        next_ptr = tmp;
    }

    /* Result is the number of ways to reach the target sum */
    seq_t result = dp_ptr[target];

#if !defined(__cplusplus)
    /* Clean up C allocations */
    free(dp_ptr);
    free(next_ptr);
#endif

    return result;
}
