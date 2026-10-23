/*
 * A100481  Greatest prime factor in A095117(n).
 * https://oeis.org/A100481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = A006530( n + A000720(n) )
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   greatest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greatest prime factor of ( n + number of primes ≤ n ) */

#include <stdlib.h>
#include <string.h>
#include "../utils/greatest_prime_factor.h"

static uint64_t max_sieve = 0;
static uint32_t *pi_table = NULL;      /* prefix prime counts: pi_table[i] = π(i) */
static uint8_t *sieve_bits = NULL;     /* 0 = prime, 1 = composite */

/* Build sieve and π(i) table up to at least 'limit' */
static void init_sieve(uint64_t limit) {
    if (limit <= max_sieve) return;

    /* Allocate new buffers */
    size_t sz = limit + 1;
    uint8_t  *new_sieve = (uint8_t *)malloc(sz);
    uint32_t *new_pi    = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!new_sieve || !new_pi)  /* allocation failure -> program abort */
        abort();
    memset(new_sieve, 0, sz);

    uint32_t count = 0;
    new_pi[0] = 0;
    if (sz > 1) new_pi[1] = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!new_sieve[i]) {
            /* i is prime */
            ++count;
            if (i <= limit / i) {          /* avoid overflow of i*i */
                for (uint64_t j = i * i; j <= limit; j += i)
                    new_sieve[j] = 1;
            }
        }
        new_pi[(size_t)i] = count;
    }

    /* Replace old buffers */
    if (pi_table)   free(pi_table);
    if (sieve_bits) free(sieve_bits);
    pi_table     = new_pi;
    sieve_bits   = new_sieve;
    max_sieve   = limit;
}

/* Public function as required by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety guard, offset starts at 1 */

    /* Ensure we have π(n) ready */
    init_sieve((uint64_t)n);

    uint32_t pi_n = pi_table[(size_t)n];          /* number of primes ≤ n */
    seq_t  value = (seq_t)n + (seq_t)pi_n;        /* n + π(n) */
    return greatest_prime_factor(value);
}
