/*
 * A180550  Primes that become a different prime under the mapping 4 <=> 9.
 * https://oeis.org/A180550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 400, u32 n <= 1559, u64 n <= 1559,
 *            u128 n <= 1559, big any n
 * Helpers:   is_prime_small, map_4_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes that become a different prime under the mapping 4 <=> 9
   (OEIS A180550).  This implementation dynamically generates primes
   and stores those that satisfy the mapping condition.  It is
   efficient enough for n up to about 10 000 while keeping the
   algorithm O(n log n). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/map_4_9.h"

static seq_t *property_primes = NULL;
static size_t property_len   = 0;
static size_t property_cap   = 0;
static idx_t  next_candidate = 2;

/* Append a value to the property list, growing the buffer if needed. */
static void append_property(seq_t val)
{
    if (property_cap == 0) {
        property_cap = 16;
        property_primes = (seq_t *)malloc(sizeof(seq_t) * property_cap);
    } else if (property_len == property_cap) {
        property_cap <<= 1;
        property_primes = (seq_t *)realloc(property_primes,
                                            sizeof(seq_t) * property_cap);
    }
    property_primes[property_len++] = val;
}

/* Generate property primes until we have at least n of them. */
static void generate_up_to(idx_t n)
{
    while ((idx_t)property_len < n) {
        seq_t candidate = (seq_t)next_candidate;
        if (is_prime_small(candidate)) {
            seq_t mapped = map_4_9(candidate);
            if (mapped != candidate && is_prime_small(mapped))
                append_property(candidate);
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but safe default */

    if ((idx_t)property_len < n)
        generate_up_to(n);

    return property_primes[n - 1];
}
