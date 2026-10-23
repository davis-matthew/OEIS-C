/*
 * A134884  Expansion of g.f. -x*(8844*x^4 +8509*x^3 -137*x^2 -209*x
 *          +3)/((67*x^2 +67*x +1)*(134*x^3 +137*x -1)).
 * https://oeis.org/A134884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 70*a(n-1) + 9112*a(n-2) + 9313*a(n-3) + 8978*a(n-4) +
 *            8978*a(n-5) + r_n with r_1=3, r_2=-209, r_3=-137, r_4=8509,
 *            r_5=8844, r_n=0 for n>5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_N 20000   /* sufficient for the required range (≈10 000) */

seq_t a(idx_t n)
{
    static seq_t cache[MAX_N+1] = {0};   /* cache[0] = 0 (dummy) */
    static idx_t max_computed = 0;       /* largest index already computed */

    if (n == 0)                     /* 0 is outside the OEIS offset */
        return 0;
    if (n > MAX_N) {
        /* The requested index exceeds the pre‑allocated range. */
        /* For the intended tests this should never happen. */
        abort();
    }

    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = 0;

            if (i >= 1) sum += cache[i-1] * 70;
            if (i >= 2) sum += cache[i-2] * 9112;
            if (i >= 3) sum += cache[i-3] * 9313;
            if (i >= 4) sum += cache[i-4] * 8978;
            if (i >= 5) sum += cache[i-5] * 8978;

            if (i == 1)
                sum += 3;
            else if (i == 2)
                sum -= 209;
            else if (i == 3)
                sum -= 137;
            else if (i == 4)
                sum += 8509;
            else if (i == 5)
                sum += 8844;
            /* for i > 5, r_n = 0 */

            cache[i] = sum;
        }
        max_computed = n;
    }
    return cache[n];
}
