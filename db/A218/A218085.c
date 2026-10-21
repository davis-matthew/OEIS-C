/*
 * A218085  Let S_5(x) denote the difference in counts of multiples of 5 in
 *          the interval [0,x), those with even digit sums in base 4 in one
 *          set, those with odd digit sums in base 4 in the other. Then a(n) =
 *          (-1)^s_4(n) *(S_5(n) -10*S_5(floor(n/16)) +5*S_5(floor(n/256))),
 *          where s_4(n) = A053737(n).
 * https://oeis.org/A218085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = (-1)^s_4(n) * (S_5(n) - 10*S_5(floor(n/16)) +
 *            5*S_5(floor(n/256)))
 * Range:     i8 n <= 2559, i16 n <= 2559, i32 n <= 2559, i64 n <= 2559,
 *            i128 n <= 2559, big any n
 * Helpers:   S5, base4_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where S_5(k) counts the multiples of 5 in [0,k) weighted by parity of base‑4 digit sums. */

#include <stdbool.h>
#include "../utils/S5.h"
#include "../utils/base4_parity.h"

seq_t a(idx_t n)
{
    seq_t s5   = S5(n);
    seq_t s5_16 = S5(n / 16);
    seq_t s5_256 = S5(n / 256);

    seq_t diff = s5 - 10 * s5_16 + 5 * s5_256;

    int p = base4_parity(n);
    seq_t sign = (p ? -1 : 1);

    return sign * diff;
}
