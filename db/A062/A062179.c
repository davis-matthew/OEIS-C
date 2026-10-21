/*
 * A062179  Harmonic mean of digits is an integer.
 * https://oeis.org/A062179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 674, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Harmonic mean of digits is an integer.
   a(n) = the n‑th positive integer whose decimal digits are all non‑zero
   and the harmonic mean of its digits is an integer. */

#include <stdlib.h>

static const seq_t LCM = 2520;            /* lcm(1,…,9) */

/* Cache of already‑found terms.  */
static seq_t *cache = NULL;
static size_t cache_size = 0;            /* number of stored terms */
static size_t cache_cap  = 0;            /* allocated capacity */

/* Next integer to test (greater than every value already checked). */
static seq_t next_candidate = 1;

/* Returns 1 iff m has no zero digit and its harmonic mean of digits
   is an integer.  Uses the property:
      H = len * LCM / Σ(LCM/d_i)  */
static int
is_harmonic_integer(seq_t m)
{
    seq_t sum = 0;        /* Σ(LCM/d_i)  */
    seq_t len = 0;        /* number of digits */

    while (m > 0) {
        seq_t d = m % 10;      /* current digit */
        if (d == 0)            /* digit zero → undefined */
            return 0;
        sum += LCM / d;
        len++;
        m = m / 10;
    }
    return ((len * LCM) % sum) == 0;
}

/* Grow the cache array to hold at least 'needed' elements.  */
static void
grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                /* allocation failed – abort */
        abort();

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t
a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1 */
        return 0;

    /* Ensure the cache contains at least n terms. */
    while (cache_size < n) {
        /* Find the next term that satisfies the condition. */
        while (!is_harmonic_integer(next_candidate))
            next_candidate++;

        grow_cache(cache_size + 1);
        cache[cache_size] = next_candidate;
        cache_size++;

        /* Advance to the next integer to test. */
        next_candidate++;
    }

    return cache[n - 1];
}
