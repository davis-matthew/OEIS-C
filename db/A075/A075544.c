/*
 * A075544  Primes which are the sum of the digital root of the first n primes
 *          for some n.
 * https://oeis.org/A075544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime which equals the sum of the digital roots of the
 *            first k primes for some k (k increased until a(n) is found)
 * Range:     u8 n <= 12, u16 n <= 804, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digital_root_uint64, ensure_capacity, is_prime_big,
 *            is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility headers */
#include "../utils/is_prime_small.h"      /* bool is_prime_small(uint64_t n); */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n); */
#include "../utils/ensure_capacity.h"   /* void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem); */
#include "../utils/digital_root_uint64.h"

/* Static state shared by all calls to a() */
static seq_t       *seq_terms = NULL;    /* array of discovered sums */
static size_t      seq_cap   = 0;         /* capacity of seq_terms */
static idx_t       seq_len   = 0;         /* number of terms stored */

static uint64_t    next_prime_candidate = 1;   /* last prime processed (starts at 1) */
static seq_t       current_sum          = 0;   /* cumulative sum of digital roots */

seq_t a(idx_t n)
{
    /* The caller may supply n < 1, but the OEIS offset is 1;
       behaviour for such n is undefined; we simply return 0. */
    if (n < 1) return 0;

    while (seq_len < n) {
        /* Find the next prime greater than next_prime_candidate */
        uint64_t candidate = next_prime_candidate + 1;
        while (!is_prime_small(candidate))
            ++candidate;
        next_prime_candidate = candidate;

        /* Update the cumulative sum of digital roots */
        current_sum += digital_root_uint64(candidate);

        /* If the sum itself is prime, record it */
        if (is_prime_big(current_sum)) {
            ensure_capacity(seq_len + 1, &seq_cap,
                            (void **)&seq_terms, sizeof(seq_t));
            seq_terms[seq_len++] = current_sum;
        }
    }

    return seq_terms[n - 1];
}
