/*
 * A104261  Sum of even digits (0,2,4,6,8) of n-th prime.
 * https://oeis.org/A104261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007605(n) - A104260(n)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t *primes = NULL;
static idx_t primes_count = 0;
static idx_t primes_cap   = 0;

static void ensure_n_primes(idx_t n)
{
    if (primes_count >= n)
        return;

    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    }

    /* Add the first prime 2 if not present yet. */
    if (primes_count == 0) {
        primes[primes_count++] = 2;
    }

    /* Start candidate at the next integer after the last prime. */
    idx_t candidate = primes[primes_count - 1] + 1;

    /* Skip even numbers greater than 2. */
    if (candidate % 2 == 0)
        ++candidate;

    while (primes_count < n) {
        if (is_prime(candidate)) {
            if (primes_count == primes_cap) {
                primes_cap *= 2;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_count++] = candidate;
        }
        candidate += 2;               /* next odd candidate */
    }
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term. */
    if (n == 0)
        return 0;

    ensure_n_primes(n);

    idx_t p = primes[n - 1];
    seq_t sum = 0;

    while (p > 0) {
        unsigned d = p % 10;
        if ((d & 1) == 0)          /* even digit */
            sum += d;
        p /= 10;
    }
    return sum;
}
