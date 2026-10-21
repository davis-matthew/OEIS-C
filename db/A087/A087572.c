/*
 * A087572  Smallest prime of the form n + (n-1) + (n-2) + ...(n-k), k < n, or
 *          0 if no such prime exists.
 * https://oeis.org/A087572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime of the form * n + (n-1) + (n-2) + … +
 *            (n-k), k < n, * or 0 if no such prime exists. * This is computed
 *            by scanning the sums in increasing order * and returning the
 *            first prime found
 * Range:     u8 n <= 24, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but guard against 0 */
        return 0;

    for (idx_t k = 0; k < n; ++k) {
        /* S(k) = (k+1)*(2*n - k)/2  */
        seq_t sum = ((seq_t)(k + 1) * ((seq_t)(2*n - k))) / 2;
        if (is_prime_big(sum))
            return sum;
    }
    return 0;
}
