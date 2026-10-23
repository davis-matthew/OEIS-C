/*
 * A041234  Numerators of continued fraction convergents to sqrt(129).
 * https://oeis.org/A041234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(129)
   Recurrence:  p_{-2}=0, p_{-1}=1,
                p_k = a_k * p_{k-1} + p_{k-2}
   where a_0 = 11 and for k≥1 the continued‑fraction
   partial quotients repeat the period
        [2, 1, 3, 1, 6, 1, 3, 1, 2, 22].
   a(n) = p_n. */

#include <stdint.h>

static const int period[10] = {2, 1, 3, 1, 6, 1, 3, 1, 2, 22};

seq_t a(idx_t n)
{
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;
    seq_t p;

    for (idx_t k = 0; k <= n; ++k)
    {
        int a_k = (k == 0) ? 11 : period[(int)((k - 1) % 10)];
        p = ((seq_t)a_k) * p_minus1 + p_minus2;

        /* shift for next step */
        p_minus2 = p_minus1;
        p_minus1 = p;
    }
    return p_minus1;
}
