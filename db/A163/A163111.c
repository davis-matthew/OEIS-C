/*
 * A163111  Prime numbers with gaps larger than 18 towards both neighboring
 *          primes.
 * https://oeis.org/A163111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 934 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute the nth prime whose gap to each neighboring prime exceeds 18
   (OEIS A163111).  The function generates successive primes using next_prime
   until the desired count of qualifying primes is reached. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Global state for memoization */
static seq_t *specials      = NULL;   /* array of qualifying primes */
static size_t specialsCap   = 0;       /* allocated capacity of specials       */
static size_t specialsSize  = 0;       /* number of primes stored             */

static uint64_t prevPrime = 0;        /* previous prime in the sequence    */
static uint64_t currPrime = 0;        /* current prime under inspection    */

/* Ensure specials has room for at least need elements */
static void ensure_specials_capacity(size_t need)
{
    if (need > specialsCap)
        grow_seq(&specials, &specialsCap, need);
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification.  If n is already computed, return directly. */
    if ((size_t)n <= specialsSize)
        return specials[(size_t)n - 1];

    /* First-time initialization: set up the first two primes. */
    if (specialsSize == 0) {
        prevPrime = 2;
        currPrime = next_prime(prevPrime);   /* first prime after 2: 3 */
    }

    /* Generate successive primes until we have at least n qualifying ones. */
    while (specialsSize < (size_t)n) {
        uint64_t nextPrime = next_prime(currPrime);   /* prime after currPrime */

        uint64_t gapBefore = currPrime - prevPrime;
        uint64_t gapAfter  = nextPrime - currPrime;

        if (gapBefore > 18 && gapAfter > 18) {
            /* Store qualifying prime */
            ensure_specials_capacity(specialsSize + 1);
            specials[specialsSize++] = (seq_t)currPrime;
        }

        /* Advance to the next pair of consecutive primes. */
        prevPrime = currPrime;
        currPrime = nextPrime;
    }

    return specials[(size_t)n - 1];
}
