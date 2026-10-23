/*
 * A014073  Number of vectors abcdefg with a,b,... >= 0, a+...+g=n,
 *          a>={b,...g}.
 * https://oeis.org/A014073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{a=1}^{n} \sum_{k=0}^{6} (-1)^k C(6,k) * C(n-a -
 *            k*(a+1) + 5, 5) (with the inner term defined only if n-a -
 *            k*(a+1) >= 0). For n=0 we have a(0)=1
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    const seq_t comb6[7] = { 1, 6, 15, 20, 15, 6, 1 };

    seq_t result = 0;

    for (idx_t a = 1; a <= n; ++a) {
        idx_t m = n - a;                 /* Remaining sum to distribute among b..g */
        seq_t pos = 0;                   /* Sum of terms with even k */
        seq_t neg = 0;                   /* Sum of terms with odd k  */

        for (int k = 0; k <= 6; ++k) {
            /* t = m - k*(a+1) must be non‑negative to contribute */
            int64_t t_signed = (int64_t)m - (int64_t)k * (int64_t)(a + 1);
            if (t_signed < 0) continue;

            idx_t t = (idx_t)t_signed;

            /* C(t + 5, 5) is the count of bounded solutions for this k */
            seq_t term = binomial(t + 5, 5);
            term *= comb6[k];

            if (k & 1) neg += term;
            else       pos += term;
        }

        seq_t contrib = pos - neg;       /* pos >= neg by definition */
        result += contrib;
    }

    return result;
}
