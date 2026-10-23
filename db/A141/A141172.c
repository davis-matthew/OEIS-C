/*
 * A141172  Primes of the form 2*x^2+2*x*y-3*y^2 (as well as of the form
 *          2*x^2+6*x*y+y^2).
 * https://oeis.org/A141172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 404, u32 n <= 404, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form 2*x^2+2*x*y-3*y^2
 * (equivalently, primes ≡ 1, 9, 25 (mod 28), plus the prime 2). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *set_cache = NULL;           /* cached primes */
static size_t set_cap   = 0;              /* allocated capacity */
static size_t set_size  = 0;              /* number of cached primes */
static idx_t  next_candidate = 3;         /* next odd number to test */

static void ensure_set_cache(idx_t n)
{
    if (n == 0)
        return;
    if (set_size >= (size_t)n)
        return;

    /* Allocate or grow cache to hold at least n elements. */
    if (set_cap < (size_t)n) {
        seq_t *tmp = seq_realloc(set_cache, set_cap, (size_t)n);
        if (!tmp)
            abort();
        set_cache = tmp;
        set_cap   = (size_t)n;
    }

    /* Insert the first prime if the cache is empty. */
    if (set_size == 0) {
        set_cache[set_size++] = (seq_t)2;
        next_candidate = 3;
    }

    /* Largest value representable in seq_t. */
    seq_t max_val = ~((seq_t)0);
    idx_t max_allowed = (idx_t)max_val;

    /* Search for remaining primes until the cache holds n primes. */
    while (set_size < (size_t)n) {
        if (next_candidate > max_allowed)
            abort();   /* further primes do not fit in seq_t */

        bool ok = false;
        idx_t r = next_candidate % 28ULL;
        if (next_candidate == 2 || r == 1 || r == 9 || r == 25)
            ok = true;

        if (ok && is_prime_big((seq_t)next_candidate))
            set_cache[set_size++] = (seq_t)next_candidate;

        next_candidate += 2;   /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0 */
    ensure_set_cache(n);
    return set_cache[n - 1];
}
