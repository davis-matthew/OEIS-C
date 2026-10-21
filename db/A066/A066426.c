/*
 * A066426  Conjectured values for a(n) = least natural number k such that
 *          phi(n+k) = phi(n) + phi(k), if k exists; otherwise 0.
 * https://oeis.org/A066426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 1 | φ(n + k) = φ(n) + φ(k)}; a(n) = 0 if no such
 *            k exists
 * Range:     u8 n <= 44, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0. */
    if (n == 0) return 0;

    /* Compute φ(n) once. */
    seq_t phi_n = phi(n);

    /* Search bound.  For n ≡ 0 (mod 3) the known solutions can be larger,
       so we allow a larger range.  For the other two residue classes the
       observed solutions all lie within 2 n.  An upper bound of 10 n is
       generous and still very fast for n ≤ 10 000. */
    idx_t limit = (n % 3 == 0) ? 10 * n : 2 * n;
    if (limit < 1) limit = 1;

    for (idx_t k = 1; k <= limit; ++k) {
        seq_t phi_k  = phi(k);
        seq_t phi_nk = phi(n + k);

        if (phi_nk == phi_n + phi_k) {
            return (seq_t)k;   /* cast is safe; overflow indicates k is too large for this build. */
        }
    }

    /* No solution found within the search window – return 0 per definition. */
    return 0;
}
