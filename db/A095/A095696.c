/*
 * A095696  T(n,5) diagonal of triangle in A095693.
 * https://oeis.org/A095696
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            ((n)(n-1)(n-2)(n-3)(n-4)(n^5+5n^4+45n^3-1605n^2+9234n-17216))/3840
 *            Equivalent binomial form (valid for n ≥ 5): a(n) = 22*C(n+5,10)
 *            + 580*C(n+4,10) + 715*C(n+3,10) + 455*C(n+2,10) - 1705*C(n+1,10)
 *            + 878*C(n,10)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 21, u64 n <= 193, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 981 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    seq_t res = 0;

    /* Binomial coefficient contributions from the generating function */
    res += (seq_t)22   * binomial(n + 5, 10);
    res += (seq_t)580  * binomial(n + 4, 10);
    res += (seq_t)715  * binomial(n + 3, 10);
    res += (seq_t)455  * binomial(n + 2, 10);
    res += (seq_t)878  * binomial(n,   10);
    res -= (seq_t)1705 * binomial(n + 1, 10);

    return res;
}
