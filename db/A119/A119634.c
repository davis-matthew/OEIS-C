/*
 * A119634  a(n) = lcm(1,...,2n+2)/2.
 * https://oeis.org/A119634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1,2,...,2n+2)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the upper bound M = 2n + 2 */
    uint64_t M = (uint64_t)n * 2 + 2;

    /* Result will be the LCM, later divided by 2 */
    seq_t result = (seq_t)1;

    /* Sieve is_prime[0..M] */
    bool *is_prime = (bool*)calloc((size_t)(M + 1), sizeof(bool));
    if (!is_prime) {
        /* Allocation failed – return 1 as fallback */
        return result;
    }

    /* Initialize the sieve */
    for (size_t i = 0; i <= M; ++i)
        is_prime[i] = true;
    if (M >= 0) is_prime[0] = false;
    if (M >= 1) is_prime[1] = false;

    /* Eratosthenes */
    for (uint64_t i = 2; i * i <= M; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= M; j += i)
                is_prime[j] = false;
        }
    }

    /* For each prime p <= M, multiply result by p^e
       where e = floor(log_p M) */
    for (uint64_t p = 2; p <= M; ++p) {
        if (!is_prime[p])
            continue;

        /* Compute e = floor(log_p M) */
        unsigned e = 0;
        uint64_t val = 1;
        while (val <= M / p) {
            val *= p;
            ++e;
        }

        seq_t p_seq = (seq_t)p;
        for (unsigned i = 0; i < e; ++i)
            result = result * p_seq;
    }

    free(is_prime);

    /* Divide by 2 to get a(n) */
    result = result / (seq_t)2;
    return result;
}
