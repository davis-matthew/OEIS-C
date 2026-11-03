/*
 * A180550  Primes that become a different prime under the mapping 4 <=> 9.
 * https://oeis.org/A180550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 400, u32 n <= 1559, u64 n <= 1559,
 *            u128 n <= 1559, big any n
 * Validated: standard, pass, 60 terms through n = 1516 (2026-11-03)
 * Helpers:   is_prime_seq, mem
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
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Map a number by swapping decimal digits 4 and 9. */
static seq_t map_4_9(seq_t n)
{
    seq_t res   = 0;
    seq_t place = 1;
    while (n > 0) {
        seq_t digit = n % 10;
        if (digit == 4)
            digit = 9;
        else if (digit == 9)
            digit = 4;
        res   += digit * place;
        place *= 10;
        n /= 10;
    }
    return res;
}

static seq_t *property_primes = NULL;
static size_t property_len   = 0;
static size_t property_cap   = 0;
static idx_t  next_candidate = 2;

/* Append a value to the property list, growing the buffer if needed. */
static void append_property(seq_t val)
{
    if (property_cap == 0) {
        property_cap = 16;
        property_primes = mem_new(seq_t, property_cap);
    } else if (property_len == property_cap) {
        property_cap <<= 1;
        property_primes = mem_resize(seq_t, property_primes, property_cap);
    }
    property_primes[property_len++] = val;
}

/* Generate property primes until we have at least n of them. */
static void generate_up_to(idx_t n)
{
    while ((idx_t)property_len < n) {
        seq_t candidate = (seq_t)next_candidate;
        if (is_prime_seq(candidate)) {
            seq_t mapped = map_4_9(candidate);
            if (mapped != candidate && is_prime_seq(mapped))
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
