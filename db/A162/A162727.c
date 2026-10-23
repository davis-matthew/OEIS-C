/*
 * A162727  G.f. is the polynomial (Product_{k=1..28} (1 - x^(3*k)))/(1-x)^28.
 * https://oeis.org/A162727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Π_{k=1..28} (1 + x + … + x^{3k-1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 43, u128 n <= 365,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equal to the coefficient of x^n in (∏_{k=1..28} (1 - x^{3k}))/(1-x)^28 */
/* Sequence is zero for n > 1190. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_N 1190        /* 3*28*29/2 - 28 */

seq_t a(idx_t n)
{
    static seq_t values[MAX_N + 1];
    static bool initialized = false;

    if (!initialized) {
        static seq_t dp[MAX_N + 1];
        static seq_t temp[MAX_N + 1];

        /* Initial DP: only 0 can be represented using 0 parts. */
        for (size_t i = 0; i <= MAX_N; ++i)
            dp[i] = 0;
        dp[0] = 1;

        for (int k = 1; k <= 28; ++k) {
            /* Each summand can contribute 0 .. 3k-1.  Length = 3k. */
            int len = 3 * k;
            seq_t window = 0;

            for (size_t i = 0; i <= MAX_N; ++i) {
                window += dp[i];
                if (i >= (size_t)len)
                    window -= dp[i - len];
                temp[i] = window;
            }

            /* Copy temporary results back to dp for next iteration. */
            for (size_t i = 0; i <= MAX_N; ++i)
                dp[i] = temp[i];
        }

        /* Store computed values for future calls. */
        for (size_t i = 0; i <= MAX_N; ++i)
            values[i] = dp[i];
        initialized = true;
    }

    if ((idx_t)MAX_N >= n)
        return values[(size_t)n];
    return 0;
}
