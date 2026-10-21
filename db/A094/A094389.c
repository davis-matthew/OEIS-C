/*
 * A094389  Last decimal digit of the odd Catalan number A038003(n).
 * https://oeis.org/A094389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = last decimal digit of the odd Catalan number A038003(n) =
 *            Catalan(2^{n-1} - 1) mod 10 For n <= 10 the values are computed
 *            explicitly; for n >= 10 the known sequence stabilises at 5
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0‑index entry unused for simplicity */
    static const seq_t small[] = {0, 1, 1, 5, 9, 5, 9, 5, 9, 7, 5};
    if (n <= 10)
        return small[(size_t)n];
    return (seq_t)5;
}
