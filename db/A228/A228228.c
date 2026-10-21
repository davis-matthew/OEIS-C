/*
 * A228228  Primes congruent to {3, 5, 13, 15} mod 16.
 * https://oeis.org/A228228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p mod 16 is 3, 5, 13 or 15
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_len = 0;

/* Build a list of all primes up to LIMIT that lie in the required residue classes. */
static void init_sequence(void) {
    const idx_t LIMIT = 5000000; /* 5·10⁶ gives >200 000 terms of this AP. */
    uint8_t *is_composite = (uint8_t*)calloc((size_t)(LIMIT + 1), 1);
    if (!is_composite) return; /* allocation failed – will return 0 */

    /* Simple sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_composite[j] = 1;
        }
    }

    /* Collect primes in the desired congruence classes */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            uint8_t r = i & 15; /* i % 16 */
            if (r == 3 || r == 5 || r == 13 || r == 15) {
                if (prime_len == prime_cap) {
                    size_t new_cap = prime_cap ? prime_cap << 1 : 64;
                    seq_t *tmp = (seq_t*)realloc(prime_list, new_cap * sizeof(seq_t));
                    if (!tmp) return; /* allocation failed */
                    prime_list = tmp;
                    prime_cap = new_cap;
                }
                prime_list[prime_len++] = (seq_t)i;
            }
        }
    }
    free(is_composite);
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* sequence starts at index 1; undefined otherwise */
    if (!initialized) init_sequence();
    if (n <= prime_len) return prime_list[n - 1];
    return (seq_t)0; /* out of range – should not happen for tested n */
}
