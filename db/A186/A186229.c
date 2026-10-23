/*
 * A186229  Expansion of (2F1( (-(1/2), 1/6); (-2/3))( 16 x) -1)/(2*x).
 * https://oeis.org/A186229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A186229 – Expansion of (2F1(-(1/2),1/6; -2/3)(16 x)-1)/(2*x)
 *  Recurrence:   (n+1)*(3n-2)*a(n) = 4*(6n+1)*(2n-1)*a(n-1)
 *  Initial value: a(0) = 1
 *  The routine evaluates the recurrence iteratively.  It works
 *  for any build where seq_t is an integer type (uint8_t … uint64_t
 *  or unsigned __int128) or an arbitrary‑precision class providing
 *  operators +, -, *, / and a constructor from integer literals.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* current value of a(k-1); start with a(0)=1 */
    seq_t value = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        if (sizeof(seq_t) <= 8)           /* built‐in small integer – use 128‑bit intermediate */
        {
            unsigned __int128 num = 4;
            num *= (6 * k + 1);
            num *= (2 * k - 1);

            unsigned __int128 den = (k + 1);
            den *= (3 * k - 2);

            unsigned __int128 prod = (unsigned __int128)value * num;
            prod /= den;
            value = (seq_t)prod;          /* truncate or cast to seq_t constructor */
        }
        else                               /* arbitrary‑precision or large integer */
        {
            seq_t num = (seq_t)4;
            num *= (6 * k + 1);
            num *= (2 * k - 1);

            seq_t den = (seq_t)(k + 1);
            den *= (3 * k - 2);

            value = (value * num) / den;   /* exact division guaranteed by recurrence */
        }
    }

    return value;
}
