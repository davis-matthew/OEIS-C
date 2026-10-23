/*
 * A027423  Number of divisors of n!.
 * https://oeis.org/A027423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = τ(n!) = ∏_{p ≤ n} ( floor(n/p)+floor(n/p²)+… + 1 )
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 49, u64 n <= 118, u128 n <= 282,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of n! for n ≥ 0. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly. */
    if (n == 0 || n == 1) return 1;

    /* Create a sieve of size n+1. */
    size_t limit = (size_t)n;
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp) return 0;               /* OOM – return 0 as a safe value */

    /* Standard Eratosthenes sieve. */
    for (size_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            if (p <= limit / p) {
                for (size_t m = p * p; m <= limit; m += p)
                    is_comp[m] = 1;
            }
        }
    }

    /* Accumulate divisor count using prime exponents in n!. */
    seq_t result = 1;
    for (size_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            idx_t pp = (idx_t)p;
            seq_t exp = 0;
            while (pp <= n) {
                exp += (seq_t)(n / pp);
                /* Avoid overflow in multiplied power. */
                if (pp > n / (idx_t)p) break;
                pp *= (idx_t)p;
            }
            result *= (exp + 1);
        }
    }

    free(is_comp);
    return result;
}
