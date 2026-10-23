/*
 * A143363  Number of ordered trees with n edges and having no protected
 *          vertices. A protected vertex in an ordered tree is a vertex at
 *          least 2 edges away from its leaf descendants.
 * https://oeis.org/A143363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence 25*(n+5)*(n+6)*a(n+5) -
 *            10*(n+5)*(5*n+21)*a(n+4) - 2*(77*n^2+613*n+1185)*a(n+3) +
 *            2*(50*n^2+253*n+312)*a(n+2) + 4*(2*n+1)*(7*n+9)*a(n+1) -
 *            4*n*(2*n+1)*a(n) = 0, with a(0)=1, a(1)=1, a(2)=1, a(3)=3,
 *            a(4)=6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

static inline seq_t init_value(idx_t i)
{
    switch (i)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)3;
        case 4: return (seq_t)6;
    }
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n <= 4)
        return init_value(n);

#ifdef __cplusplus
    /* C++ path: use std::vector so that large integer types are
       constructed correctly. */
    std::vector<seq_t> dp(n + 1);
    dp[0] = (seq_t)1;
    dp[1] = (seq_t)1;
    dp[2] = (seq_t)1;
    dp[3] = (seq_t)3;
    dp[4] = (seq_t)6;

    for (idx_t i = 5; i <= n; ++i)
    {
        idx_t k = i - 5; /* k corresponds to n in the recurrence */

        seq_t a_im1 = dp[i - 1];
        seq_t a_im2 = dp[i - 2];
        seq_t a_im3 = dp[i - 3];
        seq_t a_im4 = dp[i - 4];
        seq_t a_im5 = dp[i - 5];

        /* Integer factors computed with idx_t to avoid overflow of fB..fF */
        idx_t fB = 10 * (k + 5) * (5 * k + 21);
        idx_t fC = 2 * (77 * k * k + 613 * k + 1185);
        idx_t fD = 2 * (50 * k * k + 253 * k + 312);
        idx_t fE = 4 * (2 * k + 1) * (7 * k + 9);
        idx_t fF = 4 * k * (2 * k + 1);

        seq_t B = a_im1 * (seq_t)fB;
        seq_t C = a_im2 * (seq_t)fC;
        seq_t D = a_im3 * (seq_t)fD;
        seq_t E = a_im4 * (seq_t)fE;
        seq_t F = a_im5 * (seq_t)fF;

        /* Compute numerator as (B+C+F) - (D+E) to avoid negative intermediates */
        seq_t top = B + C + F;
        seq_t bot = D + E;
        seq_t numerator = top - bot;

        seq_t denom = (seq_t)(25 * (k + 5) * (k + 6));
        dp[i] = numerator / denom;
    }
    return dp[n];
#else
    /* C path: use malloc/calloc. Works for fundamental integer types. */
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp)
        exit(EXIT_FAILURE);

    dp[0] = (seq_t)1;
    dp[1] = (seq_t)1;
    dp[2] = (seq_t)1;
    dp[3] = (seq_t)3;
    dp[4] = (seq_t)6;

    for (idx_t i = 5; i <= n; ++i)
    {
        idx_t k = i - 5;

        seq_t a_im1 = dp[i - 1];
        seq_t a_im2 = dp[i - 2];
        seq_t a_im3 = dp[i - 3];
        seq_t a_im4 = dp[i - 4];
        seq_t a_im5 = dp[i - 5];

        idx_t fB = 10 * (k + 5) * (5 * k + 21);
        idx_t fC = 2 * (77 * k * k + 613 * k + 1185);
        idx_t fD = 2 * (50 * k * k + 253 * k + 312);
        idx_t fE = 4 * (2 * k + 1) * (7 * k + 9);
        idx_t fF = 4 * k * (2 * k + 1);

        seq_t B = a_im1 * (seq_t)fB;
        seq_t C = a_im2 * (seq_t)fC;
        seq_t D = a_im3 * (seq_t)fD;
        seq_t E = a_im4 * (seq_t)fE;
        seq_t F = a_im5 * (seq_t)fF;

        seq_t top = B + C + F;
        seq_t bot = D + E;
        seq_t numerator = top - bot;

        seq_t denom = (seq_t)(25 * (k + 5) * (k + 6));
        dp[i] = numerator / denom;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
#endif
}
