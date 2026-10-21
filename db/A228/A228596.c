/*
 * A228596  The number of simple labeled graphs on n nodes with no components
 *          of size 3.
 * https://oeis.org/A228596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=0}^{⌊n/3⌋} (-1)^m · n!/( (3!)^m m! (n-3m)! ) · 4^m ·
 *            2^{ C(n-3m, 2) }
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* maximum number of excluded components of size 3 */
    const idx_t maxm = n / 3;

    /* accumulators for even and odd terms separately */
    seq_t sum_even = 0, sum_odd = 0;

    /* C(n, m) for m = 0 used in the recurrence */
    seq_t C = 1;          /* n!/( (3!)^0 0! (n-0)! ) = 1 */
    seq_t c_power = 1;    /* 4^0 */

    /* term for m = 0 */
    {
        idx_t rem = n;
        idx_t exp = (rem >= 2) ? rem * (rem - 1) / 2 : 0;
        seq_t term = pow2(exp) * c_power;   /* C is 1 */
        sum_even += term;
    }

    /* iterate over m = 1 .. maxm */
    for (idx_t m = 1; m <= maxm; ++m) {
        /* update C = C(n, m) from C(n, m-1) */
        idx_t r = n - 3 * (m - 1);          /* r = n - 3(m-1)  > 0 */
        seq_t num = (seq_t)r * (seq_t)(r - 1) * (seq_t)(r - 2);
        seq_t den = (seq_t)6 * (seq_t)m;    /* 3! = 6 */
        C = C * num / den;

        /* update power of 4 */
        c_power = c_power * (seq_t)4;

        /* exponent for 2^C(rem,2) */
        idx_t rem = n - 3 * m;              /* remaining vertices */
        idx_t exp = (rem >= 2) ? rem * (rem - 1) / 2 : 0;

        seq_t term = C * pow2(exp) * c_power;

        if (m & 1)
            sum_odd += term;
        else
            sum_even += term;
    }

    /* all terms with even m are summed in sum_even,
       all terms with odd m are summed in sum_odd */
    return sum_even - sum_odd;
}
