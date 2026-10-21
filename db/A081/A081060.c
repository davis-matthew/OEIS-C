/*
 * A081060  Product of differences of distinct prime factors of n.
 * https://oeis.org/A081060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product(abs(p-q): p, q distinct prime factors of n)
 * Range:     u8 n <= 113, u16 n <= 869, u32 n <= 8234, u64 n <= 8234,
 *            u128 n <= 8234, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 1;
    uint64_t temp = (uint64_t)n;
    uint64_t primes[32];
    size_t cnt = 0;

    /* Factor distinct primes by trial division. */
    for (uint32_t p = 2; (uint64_t)p * (uint64_t)p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1)                /* remaining prime factor */
        primes[cnt++] = temp;

    /* If fewer than two distinct primes, product is 1. */
    if (cnt < 2) return 1;

    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        for (size_t j = i + 1; j < cnt; ++j)
        {
            uint64_t diff = (primes[j] > primes[i]) ? 
                            (primes[j] - primes[i]) :
                            (primes[i] - primes[j]);
            prod *= diff;
        }
    }
    return (seq_t)prod;
}
