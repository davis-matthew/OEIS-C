/*
 * A065400  a(n) is the greatest integer m such that (1) written in factorial
 *          base it is also readable as if it is in base n and (2) this base-n
 *          value is greater than or equal to m.
 * https://oeis.org/A065400
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum integer m whose factorial‑base digits are *
 *            readable as base‑n digits and whose base‑n interpretation is ≥
 *            m. * The algorithm chooses the largest admissible digits in
 *            increasing * position order, taking into account the weight w_i
 *            = n^{i‑1} – i!. * Positive weights allow a ful...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;          /* m in factorial base */
    seq_t fact = 1;            /* i! for current i */
    seq_t pow  = 1;            /* n^(i‑1) for current i */
    seq_t sumw = 0;            /* weighted sum Σ a_i (n^{i‑1} – i!) */

    for (idx_t i = 1; ; ++i)
    {
        /* maximal digit allowed by both bases */
        seq_t max_d = (seq_t)((i < n) ? i : (n - 1));

        /* determine sign of the weight without underflow */
        if (pow >= fact)
        {
            seq_t w = pow - fact;                /* w ≥ 0 */
            seq_t ai = max_d;                    /* use maximal digit */
            result += ai * fact;
            sumw   += ai * w;
        }
        else
        {
            seq_t penalty = fact - pow;          /* penalty > 0 */

            /* no room to add further digits */
            if (sumw == 0)
                break;

            /* how many digits can we afford at this place? */
            seq_t div = sumw / penalty;
            if (div == 0)
                break;                           /* cannot add any more */

            idx_t add  = (idx_t)div;
            if (add > (idx_t)max_d)
                add = (idx_t)max_d;

            seq_t ai = (seq_t)add;
            result += ai * fact;
            sumw   -= ai * penalty;

            if (sumw == 0)
                break;                           /* no more capacity left */
        }

        /* prepare next iteration: (i+1)! and n^i */
        fact *= (seq_t)(i + 1);
        pow  *= (seq_t)n;

        /* safety guard: we never need astronomically many positions */
        if (i > (idx_t)100000)
            break;
    }

    return result;
}
