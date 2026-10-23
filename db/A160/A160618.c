/*
 * A160618  Denominator of Laguerre(n, -1).
 * https://oeis.org/A160618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Laguerre(n, -1) Laguerre(n, -1) =
 *            Σ_{k=0}^{n} binom(n,k) / k! The denominator of the reduced
 *            fraction is n! / gcd( n!, Σ_{k=0}^{n} binom(n,k) * (n! / k!) )
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   binom, factorial, gcd, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binom.h"
#include "../utils/gcd.h"
#include "../utils/mul_mod64.h"

seq_t a(idx_t n)
{
    /* n! (exact integer) */
    seq_t fact_n = factorial(n);

    /* sum of terms modulo n! : M_n mod n! */
    seq_t sum = 0;

    /* k! as we advance */
    seq_t fact_k = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial coefficient C(n,k) */
        seq_t bin = binom(n, k);

        /* n! / k! (exact integer) */
        seq_t term_factorial = fact_n / fact_k;

        /* product = bin * (n! / k!)  modulo n! */
        seq_t prod;

#ifdef __cplusplus
        /* For native integral seq_t, use mul_mod64 to avoid overflow.
           When seq_t is an arbitrary-precision class, the multiplication
           will be performed using that class's operators. */
        if constexpr (sizeof(seq_t) <= sizeof(uint64_t))
        {
            /* cast operands to uint64_t because mul_mod64 expects them */
            prod = (seq_t)mul_mod64((uint64_t)bin,
                                     (uint64_t)term_factorial,
                                     (uint64_t)fact_n);
        }
        else
        {
            prod = bin * term_factorial;
        }
#else
        /* In plain C we assume seq_t is an integral type small enough for mul_mod64. */
        prod = (seq_t)mul_mod64((uint64_t)bin,
                                 (uint64_t)term_factorial,
                                 (uint64_t)fact_n);
#endif

        /* accumulate modulo n! */
        sum = (sum + prod) % fact_n;

        /* advance factorial: (k+1)! for next iteration */
        if (k < n) fact_k *= (idx_t)(k + 1);
    }

    /* gcd( fact_n , sum ) gives the common factor we can cancel */
    seq_t g = gcd(fact_n, sum);

    /* denominator of reduced sum is n! / g */
    return fact_n / g;
}
