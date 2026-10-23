/*
 * A161457  Number of reduced words of length n in the Weyl group A_9.
 * https://oeis.org/A161457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = coefficient of x^n in * ∏_{k=1..9} (1 + x + … + x^k) *
 *            Equivalent to the generating function ∏_{k=1..9}
 *            (1-x^{k+1})/(1-x)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Maximum exponent is 1+2+…+9 = 45 */
    enum { MAX_N = 45, M = 9 };

    /* Static storage for computed coefficients and a flag. */
    static seq_t coeff[MAX_N + 1];
    static int initialized = 0;

    if (!initialized) {
        seq_t cur[MAX_N + 1] = {0};
        cur[0] = 1;             /* start with polynomial 1 */
        int current_max = 0;     /* largest exponent with non‑zero coeff */

        for (int k = 1; k <= M; ++k) {
            seq_t next[MAX_N + 1] = {0};
            /* Multiply current polynomial by (1 + x + … + x^k). */
            for (int i = 0; i <= current_max; ++i) {
                seq_t ways = cur[i];
                if (ways == 0) continue;
                for (int add = 0; add <= k; ++add) {
                    int idx = i + add;
                    if (idx <= MAX_N)
                        next[idx] += ways;
                }
            }
            current_max += k;
            if (current_max > MAX_N) current_max = MAX_N;
            for (int i = 0; i <= current_max; ++i)
                cur[i] = next[i];
        }

        for (int i = 0; i <= MAX_N; ++i)
            coeff[i] = cur[i];

        initialized = 1;          /* mark cache ready */
    }

    if (n <= (idx_t)MAX_N)
        return coeff[(size_t)n];
    else
        return 0;
}
