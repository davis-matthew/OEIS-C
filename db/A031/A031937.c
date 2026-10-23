/*
 * A031937  Upper prime of a difference of 18 between consecutive primes.
 * https://oeis.org/A031937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = upper prime of a difference of 18 between consecutive
 *            primes (i.e., a(n) = p + 18 where p is the lower prime of a gap
 *            18). The function scans successive primes using next_prime()
 *            until the n‑th such occurrence is found and returns the
 *            corresponding upper prime
 * Range:     u8 none, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t *lower_primes = NULL;            /* store lower primes of gaps */
static size_t lower_size = 0, lower_cap = 0;   /* current size / capacity */

/* The last prime processed and the current prime being examined */
static uint64_t last_prime = 0, current_prime = 0;

/* Ensure the array has at least `need` elements; double capacity if needed. */
static void ensure_lower_cap(size_t need) {
    if (need <= lower_cap)
        return;
    size_t new_cap = (lower_cap == 0) ? 128 : lower_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(lower_primes, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    lower_primes = tmp;
    lower_cap = new_cap;
}

/* Generate lower primes until we have at least n+1 of them. */
static void generate_lower_primes(idx_t n) {
    while (lower_size <= (size_t)n) {
        /* Find next prime */
        if (current_prime == 0) {
            current_prime = next_prime(2);
        } else {
            current_prime = next_prime(current_prime + 1);
        }

        /* Check if the gap from last_prime to current_prime is exactly 18 */
        if (last_prime != 0 && (current_prime - last_prime) == 18) {
            ensure_lower_cap(lower_size + 1);
            lower_primes[lower_size++] = (seq_t)last_prime;
        }

        /* Advance to the next prime */
        last_prime = current_prime;
    }
}

/* Public function required by the specification. */
seq_t a(idx_t n) {
    generate_lower_primes(n);
    return (seq_t)lower_primes[(size_t)n] + (seq_t)18;
}
