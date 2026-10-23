/*
 * A193215  Number of Dyck paths of semilength n having the property that the
 *          heights of the first and the last peaks do not disagree.
 * https://oeis.org/A193215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{i=1..n-1} A000958(i) Recurrence
 *            2*n*(5*n-11)*a(n) = 3*(15*n^2-53*n+40)*a(n-1) -
 *            3*(5*n^2-21*n+20)*a(n-2) - 2*(2*n-5)*(5*n-6)*a(n-3) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static idx_t  cache_size = 0;   /* index of last computed value */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache && cache_cap > n)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Initialise the base cases a(0)=1, a(1)=1, a(2)=2. */
static void init_base(void)
{
    if (cache_size)
        return;
    ensure_cache(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache_size = 2;
}

seq_t a(idx_t n)
{
    init_base();

    if (n <= cache_size)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        /* Use 128‑bit intermediates for small seq_t types
           to avoid overflow of uint64_t intermediates. */
        if (sizeof(seq_t) <= 8) {
            __int128 tn = i;
            __int128 left   = 2 * tn * (5 * tn - 11);
            __int128 term1 = (15 * tn * tn - 53 * tn + 40) * (__int128)cache[i-1];
            term1 *= 3;
            __int128 term2 = (5 * tn * tn - 21 * tn + 20) * (__int128)cache[i-2];
            term2 *= 3;
            __int128 term3 = (2 * tn - 5) * (5 * tn - 6) * (__int128)cache[i-3];
            term3 *= 2;
            __int128 rhs = term1 - term2 - term3;
            cache[i] = (seq_t)(rhs / left);
        } else { /* seq_t is an arbitrary‑precision type */
            seq_t tn = (seq_t)i;
            seq_t left = 2 * tn * ((seq_t)5 * tn - (seq_t)11);
            seq_t term1 = ( (15 * tn * tn - 53 * tn + (seq_t)40) * cache[i-1] );
            term1 = 3 * term1;
            seq_t term2 = ( (5 * tn * tn - 21 * tn + (seq_t)20) * cache[i-2] );
            term2 = 3 * term2;
            seq_t term3 = ( (2 * tn - 5) * (5 * tn - 6) * cache[i-3] );
            term3 = 2 * term3;
            seq_t rhs = term1 - term2 - term3;
            cache[i] = rhs / left;
        }
    }

    cache_size = n;
    return cache[n];
}
