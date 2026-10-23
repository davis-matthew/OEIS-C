/*
 * A018763  Divisors of 992.
 * https://oeis.org/A018763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 992 in increasing order
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   divisors_of_992
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/divisors_of_992.h"

/* Sequence A018763: divisors of 992 in increasing order.
   Off‑by‑one: a(1) is the first divisor. */
seq_t a(idx_t n)
{
    /* The sequence has only 12 terms; return 0 for n > 12. */
    const seq_t *divs = divisors_of_992();
    if (n == 0 || n > 12) {
        return 0;
    }
    return divs[n - 1];
}
