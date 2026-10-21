/*
 * A050473  Smallest k such that phi(k+n) = 2*phi(k).
 * https://oeis.org/A050473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least k such that φ(k+n) = 2·φ(k).  It is known that
 * such a k always exists and satisfies k ≤ 2·n.  We simply test
 * k = 1 .. 2·n until the condition holds. */
#include "../utils/phi.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; returning 0 for n = 0 is harmless. */
    if (n == 0U)
        return (seq_t)0;

    /* Upper bound on k derived from Makowski's result (k ≤ 2n). */
    idx_t limit = 2 * n;

    for (idx_t k = 1; k <= limit; ++k)
    {
        seq_t phi_k   = phi(k);         /* φ(k)  */
        seq_t phi_knp = phi(k + n);     /* φ(k+n) */

        /* 2·φ(k) fits safely into seq_t for k ≤ limit. */
        seq_t double_phi_k = (seq_t)2 * phi_k;

        if (phi_knp == double_phi_k)
            return (seq_t)k;
    }

    /* In theory this point should never be reached. */
    return (seq_t)0;
}
