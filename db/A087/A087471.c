/*
 * A087471  Final digit resulting from iterations of the product of the two
 *          numbers formed from the alternating digits of n.
 * https://oeis.org/A087471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(f(n)), where f(n) is Murthy's function: * f(n) =
 *            (number formed by digits at odd positions) * * (number formed by
 *            digits at even positions) * The sequence a(n) is the terminal
 *            single digit obtained by * iterating f until a one‑digit number
 *            remains
 * Range:     u8 n <= 303, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   murthy_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/murthy_func.h"

/* Main sequence function.  Input index n (offset 1). */
seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    while (v >= 10)                /* repeat until single digit */
        v = murthy_func(v);
    return v;
}
