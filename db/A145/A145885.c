/*
 * A145885  a(n) = (n-1)^2*binomial(2n,n)/(2*(n+1)).
 * https://oeis.org/A145885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^2 * Catalan(n) / 2 (Offset: 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset, return 0 */

    seq_t cat    = catalan(n);                    /* Catalan(n) */
    seq_t factor = ((seq_t)(n - 1) * (n - 1));     /* (n-1)^2 */

    /* To avoid overflow, divide an even factor by 2 first */
    if ((cat & 1) == 0) {                     /* cat even */
        return (cat / 2) * factor;
    } else {                                   /* factor must be even */
        return cat * (factor / 2);
    }
}
