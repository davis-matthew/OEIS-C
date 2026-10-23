/*
 * A010138  Continued fraction for sqrt(52).
 * https://oeis.org/A010138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 9725, u16 n <= 9725, u32 n <= 9725, u64 n <= 9725,
 *            u128 n <= 9725, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence A010138: Continued fraction for √52.
   The expansion is 7 + 1/(4 + 1/(1 + 1/(2 + 1/(1 + 1/(4 + 1/(14 + …))))) ).
   The terms are:
        a(0) = 7,
        and for n ≥ 1 the sequence repeats every 6 terms:
            4, 1, 2, 1, 4, 14.
   Hence a(n) = period[(n‑1) mod 6] with
        period[0] = 4, period[1] = 1, period[2] = 2,
        period[3] = 1, period[4] = 4, period[5] = 14. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;

    static const seq_t period[6] =
    {
        (seq_t)4, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)4, (seq_t)14
    };

    return period[(size_t)((n - 1) % 6)];
}
