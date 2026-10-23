/*
 * A100066  Expansion of x/((1-x)*sqrt(1-4*x^2)).
 * https://oeis.org/A100066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binomial(2k, k) (with a(0)=0).
 *            The recurrence a(n+1)=Sum_{k=0..floor(n/2)} binomial(2k,k) is
 *            used for computation. For small integer seq_t values arithmetic
 *            is performed using 128‑bit temporaries to avoid overflow; for
 *            arbitrary‑precision seq_t the recurrence is a...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 68, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t etc. */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* Small seq_t types: use 128‑bit temporaries. */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        seq_t sum = 1;        /* k = 0 contributes C(0,0) = 1 */
        seq_t term = 1;       /* current binomial C(2k,k) for current k */
        idx_t kmax = (n - 1u) / 2u;

        for (idx_t k = 1; k <= kmax; ++k)
        {
            /* term = term * (4*k-2) / k  using 128‑bit intermediate */
            __uint128_t prod = ( (__uint128_t)term * (4u * k - 2u) ) / k;
            term = (seq_t)prod;
            sum += term;
        }
        return sum;
    }

    /* Arbitrary‑precision seq_t: use the recurrence directly. */
    seq_t sum = 1;        /* k = 0 contributes C(0,0) = 1 */
    seq_t term = 1;       /* current binomial C(2k,k) */
    idx_t kmax = (n - 1u) / 2u;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        term = term * (4u * k - 2u);
        term = term / k;          /* exact division */
        sum += term;
    }
    return sum;
}
