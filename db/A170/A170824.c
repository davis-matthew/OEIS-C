/*
 * A170824  a(n) = product of distinct primes of form 6k+1 that divide n.
 * https://oeis.org/A170824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of distinct primes of form 6k+1 that divide n
 * Range:     u8 n <= 258, u16 n <= 7808, u32 n <= 7808, u64 n <= 7808,
 *            u128 n <= 7808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* seq_t and idx_t are defined by the build; do not redefine them */

/* Compute the product of distinct primes ≡ 1 (mod 6) dividing n.
 * For n = 1 the product is defined to be 1 by the OEIS entry. */
seq_t a(idx_t n)
{
    /* Handle the special case immediately to avoid unnecessary work. */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* Trial division by all possible divisors up to sqrt(temp). */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if ((temp % p) != 0)
            continue;

        /* p is a prime divisor of temp.  Multiply if it satisfies p % 6 == 1. */
        if ((p % 6) == 1)
            result *= (seq_t)p;

        /* Dividing out all powers of p to keep p distinct. */
        while (temp % p == 0)
            temp /= p;
    }

    /* If temp > 1, it is a prime factor larger than sqrt(original n). */
    if (temp > 1 && (temp % 6) == 1)
        result *= (seq_t)temp;

    return result;
}
