/*
 * A142362  Primes congruent to 11 mod 47.
 * https://oeis.org/A142362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 11 (mod 47)
 * Range:     u8 n <= 2, u16 n <= 134, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t* primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void
ensure_capacity(size_t new_cap)
{
    if (new_cap <= primes_cap)
        return;
    size_t cap = primes_cap > 0 ? primes_cap : 8;
    while (cap < new_cap)
        cap <<= 1;
    seq_t* new_arr = (seq_t*)realloc(primes, cap * sizeof(seq_t));
    /* In the unlikely event of allocation failure we simply abort,
       as the caller expects correct behaviour for all n. */
    primes = new_arr;
    primes_cap = cap;
}

seq_t a(idx_t n)
{
    while (primes_len < (size_t)n) {
        seq_t candidate;
        if (primes_len == 0)
            candidate = (seq_t)11;
        else
            candidate = primes[primes_len - 1] + (seq_t)47;

        /* Search forward by steps of 47 until a prime is found. */
        while (!is_prime_big(candidate)) {
            candidate += (seq_t)47;
        }

        ensure_capacity(primes_len + 1);
        primes[primes_len++] = candidate;
    }

    return primes[n - 1];
}
