/*
 * A142697  Primes congruent to 52 mod 57.
 * https://oeis.org/A142697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   this implementation finds the n‑th prime that satisfies p ≡ 52
 *            (mod 57)
 * Range:     u8 n <= 2, u16 n <= 179, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It uses a simple cache of already found primes; a primality test is done by    */
/* the helper is_prime_big(seq_t n) provided by the utils.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;        /* array of found primes   */
static size_t primes_sz = 0;       /* number of primes cached */
static size_t primes_cap = 0;      /* current capacity        */
static seq_t last_candidate = (seq_t)52;  /* start of the progression */

static void grow_primes(void)
{
    size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
    /* cast for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds – offset is 1 */
    /* Extend the cached list until it contains n primes */
    while (primes_sz < (size_t)n) {
        last_candidate += (seq_t)57;
        if (is_prime_big(last_candidate)) {
            if (primes_sz == primes_cap) grow_primes();
            primes[primes_sz++] = last_candidate;
        }
    }
    return primes[n - 1];
}
