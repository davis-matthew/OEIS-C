/*
 * A168012  a(n) = sum of all divisors of all numbers k such that n^2 <= k <
 *          (n+1)^2.
 * https://oeis.org/A168012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=n^2}^{(n+1)^2-1} sigma(k) * = S((n+1)^2-1) -
 *            S(n^2-1), where * S(m) = Σ_{i=1}^{m} i * ⌊m/i⌋ * (sigma is the
 *            sum of divisors)
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 730, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_sigma_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_sigma_prefix.h"

seq_t a(idx_t n)
{
    /* Calculate the two upper limits of the intervals */
    idx_t n1  = n + 1;
    idx_t M1  = n1 * n1 - 1;           // (n+1)^2 - 1
    idx_t M2  = n * n - 1;             // n^2 - 1

    seq_t S1 = sum_sigma_prefix(M1);
    seq_t S2 = sum_sigma_prefix(M2);

    return S1 - S2;
}
