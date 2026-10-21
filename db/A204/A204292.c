/*
 * A204292  a(n) = binomial(n, d(n)), where d(n) = A000005(n) is the number of
 *          divisors of n.
 * https://oeis.org/A204292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, d(n)) where d(n) = divisor_count_u64(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 419, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 we return 0
       to avoid undefined behaviour in callers that might request
       a(0). */
    if (n == 0) return (seq_t)0;

    /* Compute the number of divisors of n */
    idx_t d = divisor_count_u64(n);

    /* Ensure k = min(d, n - d) for the multiplicative
       binomial formula.  This reduces the number of iterations
       and keeps intermediate values as small as possible. */
    idx_t k = d;
    if (k > n - k) k = n - k;

    /* Compute C(n,k) multiplicatively, using a 128‑bit intermediate
       for the single multiplication before the division.  This
       protects against overflow of the seq_t intermediate (which may
       be uint64_t or an arbitrary-precision type). */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* temp holds result * (n - k + i) before the division.
           The product fits into 128 bits for all inputs that are
           relevant for this sequence. */
        __int128 temp = (__int128)result * ( (__int128)n - k + i );
        result = (seq_t)(temp / i);
    }

    return result;
}
