/*
 * A127835  (Order of Galois group of Chebyshev polynomial)/(order of
 *          polynomial); or A124827(n)/n.
 * https://oeis.org/A127835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, more
 * Formula:   a(n) = phi(n) if n is not divisible by 8, a(n) = phi(n)/2 if n
 *            is divisible by 8
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function calc_phi from ../utils/calc_phi.h. */
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Offset is 1, but behaviour for n==0 is irrelevant */
    if (n == 0) return 0;

    seq_t phi_val = calc_phi((seq_t)n);

    /* Divide by 2 only when n is a multiple of 8 */
    if ((n & 7u) == 0) {
        phi_val /= 2;
    }

    return phi_val;
}
