/*
 * A136104  A007318 * A002110; a(n) = Sum_{k=0..n} binomial(n,k)*A002110(k).
 * https://oeis.org/A136104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * primorial(k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; term_0=1; term_{k+1}=term_k * (n-k)/(k+1) * prime_{k+1} */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* term_k : current binomial‑transform term
       sum  : running total for a(n) */
    seq_t term = 1;
    seq_t sum  = term;           /* a(0) = 1, also used for other n */

    for (idx_t k = 0; k < n; ++k)
    {
        /* Multiply by (n-k)/(k+1) */
        term = term * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Multiply by the (k+1)-th prime (A002110 increment) */
        seq_t prime = (seq_t)get_prime(k + 1);
        term = term * prime;

        /* Add to total */
        sum += term;
    }

    return sum;
}
