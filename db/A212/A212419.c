/*
 * A212419  Size of the equivalence class of S_n containing the identity
 *          permutation under transformations of positionally adjacent
 *          elements of the form abc <--> acb <--> bac <--> cba, where a<b<c.
 * https://oeis.org/A212419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n<3, otherwise a(n) = n! - C(floor((n-1)/2)) -
 *            C(floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(k) is the Catalan number. */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n < 3U) return (seq_t)1;

    seq_t fact = 1;
    for (idx_t i = 2U; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t c1 = catalan((n - 1U) / 2U);
    seq_t c2 = catalan(n / 2U);

    return fact - c1 - c2;
}
