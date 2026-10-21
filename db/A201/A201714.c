/*
 * A201714  Primes of the form 2n^2 - 7.
 * https://oeis.org/A201714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 2*k^2 - 7
 * Range:     u8 none, u16 n <= 44, u32 n <= 77, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

static seq_t cache[20000];     /* storage for found primes, enough for n≈10000 */
static idx_t cache_len = 0;    /* number of primes stored so far */
static uint64_t candidate = 1; /* current integer k to test */

/* Return the n-th prime of the form 2*k^2 - 7, with n≥1. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, guard against caller error */

    /* Generate primes until we have at least n entries. */
    while (cache_len < n) {
        seq_t val = 2 * (seq_t)candidate * candidate - 7;
        if (is_prime_big(val)) {
            /* We trust that 20000 is sufficient for n up to about 10000
               as required by the problem statement. */
            cache[cache_len++] = val;
        }
        ++candidate;
    }

    return cache[n - 1];
}
