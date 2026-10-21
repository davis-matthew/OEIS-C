/*
 * A089849  Number of fixed points in range [A014137(n-1)..A014138(n-1)] of
 *          permutation A069772.
 * https://oeis.org/A089849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = binomial(2n,n), a(2n+1) = catalan(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Even indices: a(2n) = C(2n,n) */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;          /* n = 2k */
        return binom(2 * k, k);
    }
    /* Odd indices:  a(2n+1) = Catalan(n) */
    else {
        idx_t k = (n - 1) >> 1;    /* n = 2k+1 */
        return catalan(k);
    }
}
