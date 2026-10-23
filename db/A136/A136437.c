/*
 * A136437  a(n) = prime(n) - k! where k is the greatest number such that k!
 *          <= prime(n).
 * https://oeis.org/A136437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - k! where k is the largest integer with k! <=
 *            prime(n)
 * Range:     u8 n <= 46, u16 n <= 4774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Precompute factorials up to 20! (sufficient for n up to many millions). */
    static seq_t fact[21];
    static bool fact_init = false;
    if (!fact_init) {
        fact[0] = 1;
        for (idx_t i = 1; i <= 20; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_init = true;
    }

    /* Get the n-th prime. */
    uint32_t p = get_prime(n);

    /* Determine the greatest k with k! <= p. */
    int k = 0;
    for (int i = 1; i <= 20; ++i) {
        if (fact[i] <= (seq_t)p)
            k = i;
        else
            break;
    }

    /* a(n) = prime(n) - k! . */
    return (seq_t)p - fact[k];
}
