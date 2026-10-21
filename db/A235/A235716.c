/*
 * A235716  Average of twin prime pairs n having their decimal expansion of
 *          the form abcabc or abcabc0 such that n contains three twin primes
 *          as divisors.
 * https://oeis.org/A235716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235716: n = 30030 * k  with  k = 1,2,3,…  and
   both  n – 1  and  n + 1  prime.  The terms are generated
   in increasing order of k and cached for repeated calls. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache   = NULL;   /* cached terms   */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* capacity of the cache   */
static idx_t  next_k   = 1;     /* k value to test next     */

/* Append a new term, enlarging the cache if necessary. */
static void append_term(seq_t val)
{
    if (cache_len == 0 || val != cache[cache_len-1]) {   /* avoid duplicates */
        ensure_capacity(cache_len + 1, &cache_cap,
                        (void **)&cache, sizeof(seq_t));
        cache[cache_len++] = val;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* current candidate */
        seq_t val = (seq_t)30030 * (seq_t)next_k;   /* val = 30030·k */

        /* test the twin‑prime condition */
        if (is_prime_big(val - 1) && is_prime_big(val + 1))
            append_term(val);

        ++next_k;
    }

    return cache[n-1];    /* zero‑based indexing into the cache */
}
