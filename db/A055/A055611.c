/*
 * A055611  n written efficiently in natural numbers base, i.e., in form
 *          ...wxyz where n = 1*z + 2*y + 3*x + 4*w + ... with z <= 1, y < 2,
 *          x < 3, w < 4, ...
 * https://oeis.org/A055611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the base‑(natural‑numbers) representation of n with
 *            digits d_k … d_1 satisfying 1 ≤ d_k < k (k ≥ 1), 0 ≤ d_i < i for
 *            i > 1, and z ≤ 1 for the least significant digit. The integer
 *            a(n) is the decimal number obtained by concatenating the digits
 *            d_k … d_1
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   digit_len, max_sum_pos, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/max_sum_pos.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, but safe */

    seq_t target   = (seq_t)n;
    int    k       = 1;

    /* find minimal number of positions k with max_sum_pos(k) >= n   */
    while (max_sum_pos(k) < target) ++k;

    unsigned char digits[256];          /* enough for all relevant k   */
    seq_t remaining = target;

    /* determine digits from most significant to least */
    for (int i = k; i >= 1; --i)
    {
        int lo = (i == k) ? 1 : 0;          /* highest position must be >0   */
        int hi = (i == 1) ? 1 : i - 1;     /* max allowed digit            */

        int chosen = -1;
        for (int d = lo; d <= hi; ++d)
        {
            seq_t rem = remaining - (seq_t)d * i;
            if (rem < 0) break;            /* larger d will only reduce rem */

            seq_t max_low = max_sum_pos(i - 1);
            if (rem <= max_low)            /* feasible lower part          */
            {
                chosen   = d;
                remaining = rem;
                break;
            }
        }
        digits[i] = (unsigned char)chosen;
    }

    /* build the decimal value from the digits */
    seq_t result = 0;
    for (int i = k; i >= 1; --i)
    {
        seq_t d   = (seq_t)digits[i];
        int   len = digit_len(d);
        result = result * pow10((idx_t)len) + d;
    }

    return result;
}
