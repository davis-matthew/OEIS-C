/*
 * A126993  a(n) = binomial(prime(n+3), prime(n)).
 * https://oeis.org/A126993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( prime(n+3), prime(n) ) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 22, u128 n <= 60,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* primes are 1‑indexed in the OEIS definition */
    idx_t p = (idx_t)get_prime(n);     /* prime(n)   */
    idx_t q = (idx_t)get_prime(n + 3); /* prime(n+3) */

    /* binomial(q, p) = binomial(q, q-p).  Let k = q-p. */
    idx_t k = q - p;
    if (k == 0) /* p == q, happens only for n=1 (primes 2 and 5) */
        return 1;

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* Numerator term (q - k + i) and denominator i */
        seq_t num = (seq_t)(q - k + i);
        seq_t den = (seq_t)i;

        /* Reduce gcd(num,den) first */
        seq_t g = gcd(num, den);
        num /= g;     den /= g;

        /* Reduce gcd(result,den) */
        g = gcd(result, den);
        result /= g;  den /= g;

        /* After reductions, den should be 1.
           Multiply the remaining numerator. */
        /* If den is still >1, multiply then divide – it must be exact */
        result = result * num;
        if (den != 1)
            result = result / den;
    }

    return result;
}
