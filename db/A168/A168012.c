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
 * Validated: standard, pass, 52 terms through n = 889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t sum_sigma_prefix(idx_t M)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= M; )
    {
        idx_t q     = M / i;            // common quotient
        idx_t last  = M / q;            // last i with same q
        seq_t cnt   = (seq_t)(last - i + 1);
        seq_t sum_i = ((seq_t)(i + last) * cnt) / 2; // sum of i..last
        sum += sum_i * q;              // add contribution
        i = last + 1;
    }
    return sum;
}

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
