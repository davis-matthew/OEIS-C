/*
 * A048073  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{min(\lfloor m/2\rfloor,\;H)}
 *            \binom{m}{2k}\binom{H}{k}, where m = \lfloor (n+2)/3 \rfloor and
 *            H = n - m. This counts the non‑empty subsets of {1,…,n} in which
 *            exactly 2/3 of the elements are \leq (n+2)/3
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset starts at 1 */

    /* m = floor((n+2)/3), H = n - m */
    idx_t m = (n + 2) / 3;
    idx_t H = n - m;

    /* tmax = min(floor(m/2), H) */
    idx_t tmax = m / 2;
    if (tmax > H) tmax = H;

    seq_t result = 0;

    /* sum over k = number of elements > m (k = s/3) */
    for (idx_t k = 1; k <= tmax; ++k)
    {
        /* Compute C(m, 2k) */
        seq_t binom_m = 1;
        for (idx_t i = 1; i <= 2 * k; ++i)
        {
            binom_m = binom_m * (seq_t)(m - 2 * k + i) / (seq_t)i;
        }

        /* Compute C(H, k) */
        seq_t binom_H = 1;
        for (idx_t i = 1; i <= k; ++i)
        {
            binom_H = binom_H * (seq_t)(H - k + i) / (seq_t)i;
        }

        result += binom_m * binom_H;
    }

    return result;
}
