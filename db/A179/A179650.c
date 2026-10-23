/*
 * A179650  a(n) = (n-th prime) mod (n-th nonprime).
 * https://oeis.org/A179650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th prime) mod (n-th nonprime)
 * Range:     u8 n <= 54, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

/* Cache for nonprime numbers (composites plus 1). */
static seq_t *nonprime_cache   = NULL;   /* array of seq_t */
static size_t nonprime_cap     = 0;       /* allocated capacity */
static idx_t  nonprime_filled  = 0;       /* number of elements stored */
static idx_t  next_check       = 1;       /* next integer to test for nonprime */

/* Ensure that nonprime_cache holds at least ‘need’ elements. */
static void
ensure_nonprime(idx_t need)
{
    if (nonprime_filled >= need)
        return;

    /* Grow capacity to a power of two at least ‘need’ */
    size_t cap = nonprime_cap ? nonprime_cap : 128;
    while (cap < (size_t)need)
        cap <<= 1;

    if (cap != nonprime_cap) {
        seq_t *tmp = (seq_t *)realloc(nonprime_cache, cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed – leave cache unmodified */
            return;
        nonprime_cache = tmp;
        nonprime_cap   = cap;
    }

    /* Append nonprime numbers until we have ‘need’ of them. */
    while (nonprime_filled < need) {
        if (!is_prime(next_check))
            nonprime_cache[nonprime_filled++] = (seq_t)next_check;
        ++next_check;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety for out-of-range */

    seq_t prime = (seq_t)get_prime(n);           /* nth prime (1‑based) */
    ensure_nonprime(n);                          /* ensure nth nonprime cached */
    seq_t nonprime = nonprime_cache[(size_t)(n - 1)]; /* nth nonprime */
    return prime % nonprime;
}
