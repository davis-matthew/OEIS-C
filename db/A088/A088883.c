/*
 * A088883  Primes which when concatenated with their reverse and incremented
 *          by 2 yield a new prime.
 * https://oeis.org/A088883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 188, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, isPrimeGeneric, is_prime_small, next_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that concat(p, reverse(p)) + 2 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers (available in the build environment). */
#include "../utils/rev.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_small.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/concat_parts.h"

/* Static state to memoize results across calls. */
static seq_t *seq_results = NULL;      /* array of sequence values */
static size_t seq_cnt = 0;             /* number of values stored */
static size_t seq_cap = 0;             /* allocated capacity */
static uint64_t current_prime = 1;     /* last prime inspected */

/* Ensure the results array can hold at least 'new_cap' elements. */
static void ensure_seq_cap(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    size_t grow = seq_cap ? seq_cap * 2 : 8;      /* start with small size */
    if (grow < new_cap)
        grow = new_cap;
    seq_t *tmp = (seq_t *)realloc(seq_results, grow * sizeof(seq_t));
    if (!tmp) exit(1);   /* out of memory */
    seq_results = tmp;
    seq_cap = grow;
}

/* Append a new value to the results array. */
static void append_seq(seq_t val)
{
    ensure_seq_cap(seq_cnt + 1);
    seq_results[seq_cnt++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 but return 0 to avoid UB */

    /* Generate until we have at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* Find next prime 'current_prime' of the form 6k+1 (or 7). */
        do {
            current_prime = next_prime(current_prime);
        } while (current_prime % 6 != 1);

        /* Form concat(p, reverse(p)) + 2 and test for primality. */
        uint64_t rev_p = rev(current_prime);
        seq_t conc = concat_parts(current_prime, rev_p);
        conc += (seq_t)2;
        if (isPrimeGeneric(conc))
            append_seq((seq_t)current_prime);
    }
    return seq_results[n - 1];
}
