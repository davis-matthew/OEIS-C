/*
 * A162288  Number of reduced words of length n in the Weyl group D_11.
 * https://oeis.org/A162288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 110, u64 n <= 110, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): Number of reduced words of length n in the Weyl group D_11.
   A(n) is the coefficient of x^n in
       (1 + x + ... + x^10) * Π_{i=1}^{10} (1 + x + ... + x^{2i-1}).
   Equivalent to counting the number of ways to write n as the sum
   of one integer from each of the 11 sets:
       {0,…,10}, {0,1}, {0,…,3}, {0,…,5}, {0,…,7}, {0,…,9},
       {0,…,11}, {0,…,13}, {0,…,15}, {0,…,17}, {0,…,19}.
   This DP evaluates the coefficient exactly using integer arithmetic.
*/

#include <stddef.h>

#define MAX_N 110  /* Maximum exponent with non‑zero coefficient */

static const unsigned char factor_sizes[11] = {
    11,  /* f(11)   : 0 … 10   (11 terms)       */
    2,   /* f(2)    : 0 … 1    (2 terms)        */
    4,   /* f(4)    : 0 … 3    (4 terms)        */
    6,   /* f(6)    : 0 … 5    (6 terms)        */
    8,   /* f(8)    : 0 … 7    (8 terms)        */
    10,  /* f(10)   : 0 … 9    (10 terms)       */
    12,  /* f(12)   : 0 … 11   (12 terms)       */
    14,  /* f(14)   : 0 … 13   (14 terms)       */
    16,  /* f(16)   : 0 … 15   (16 terms)       */
    18,  /* f(18)   : 0 … 17   (18 terms)       */
    20   /* f(20)   : 0 … 19   (20 terms)       */
};

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n > MAX_N) return 0;          /* Coefficients vanish for n > 110 */

    size_t N = (size_t)n;
    static seq_t dp[MAX_N + 1];
    static seq_t ndp[MAX_N + 1];

    /* Initialise the DP array.  dp[s] stores the number of ways to reach
       sum s using the factors processed so far. */
    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Convolute with each factor in turn */
    for (int fi = 0; fi < 11; ++fi) {
        unsigned m = factor_sizes[fi];

        /* Zero the temporary array */
        for (size_t i = 0; i <= N; ++i) ndp[i] = 0;

        for (size_t s = 0; s <= N; ++s) {
            seq_t val = dp[s];
            if (!val) continue;                  /* Skip zero contributions */

            for (unsigned e = 0; e < m; ++e) {
                size_t t = s + e;
                if (t > N) break;                 /* Remaining e are too large */
                ndp[t] += val;
            }
        }

        /* Copy the result back into dp for the next factor */
        for (size_t i = 0; i <= N; ++i) dp[i] = ndp[i];
    }

    return dp[N];
}
