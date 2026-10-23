/*
 * A104588  Product of primes less than or equal to sqrt(n).
 * https://oeis.org/A104588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes ≤ ⌊√n⌋
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* integer square‑root: floor(sqrt(n)) using only integer arithmetic */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1))
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t root = int_sqrt(n);
    if (root < 2)            /* no primes ≤ 1 */
        return (seq_t)1;

    /* Allocate an array to store discovered primes up to 'root'.
       The worst case size is 'root-1' (for i=2..root). */
    uint32_t *primes = (uint32_t*)malloc((size_t)(root) * sizeof(uint32_t));
    if (!primes)
        return (seq_t)1;     /* unlikely, but safe fallback */

    idx_t count = 0;          /* number of primes found */
    seq_t product = 1;        /* accumulating product */

    for (idx_t i = 2; i <= root; ++i) {
        bool is_prime = true;
        for (idx_t j = 0; j < count; ++j) {
            uint32_t p = primes[j];
            if ((idx_t)p * p > i)
                break;
            if (i % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[count++] = (uint32_t)i;
            product *= (seq_t)i;
        }
    }

    free(primes);
    return product;
}
