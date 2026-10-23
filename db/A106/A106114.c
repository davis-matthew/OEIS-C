/*
 * A106114  Primes with minimal digit > 3.
 * https://oeis.org/A106114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime whose decimal representation contains only
 *            digits ≥ 4. The function returns the term for any n ≥ 1
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digits_ge_four, is_prime_uint64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is memoised and works for all builds of seq_t and idx_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_uint64.h"
#include "../utils/seq_realloc.h"
#include "../utils/digits_ge_four.h"

static seq_t *primes = NULL;          /* cached primes */
static idx_t  count   = 0;            /* number of primes stored */
static idx_t  capacity = 0;           /* current allocation capacity */
static uint64_t next_candidate = 5;   /* next odd number to test */

/* Grow the primes array to accommodate at least capacity+1 elements. */
static void grow_primes(void)
{
    if (count >= capacity) {
        capacity = (capacity == 0) ? 8 : capacity * 2;
        /* seq_realloc returns the new pointer. */
        primes = seq_realloc(primes, capacity);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but safe fallback */

    while (count < n) {
        /* Find the next odd candidate that is prime and has all digits ≥ 4. */
        while (!(is_prime_uint64(next_candidate) && digits_ge_four(next_candidate))) {
            next_candidate += 2;   /* stay odd */
        }

        grow_primes();

        /* Store the prime (automatic conversion to seq_t). */
        primes[count++] = (seq_t)next_candidate;

        /* Prepare next candidate, keeping it odd. */
        next_candidate += 2;
    }

    return primes[n - 1];
}
