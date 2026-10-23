/*
 * A184869  Numbers k such that floor[(k-1/2)*(2+2^(1/2))+1/2] is prime.
 * https://oeis.org/A184869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that floor[(k-1/2)*(2+sqrt(2))+1/2] is prime. This
 *            implementation finds k by testing candidates and caching the
 *            resulting terms
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 1157, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* Cache for computed terms.  The cache grows as needed. */
static seq_t *seq_cache  = NULL;
static size_t cache_size   = 0;   /* number of terms stored */
static size_t cache_capacity = 0;
static idx_t next_k_global = 1;   /* next k to test for membership */

/* Ensure at least n terms have been computed and cached. */
static void ensure_terms(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Expand cache capacity if necessary. */
    if (cache_capacity < n) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        seq_cache = tmp;
        cache_capacity = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_size < n) {
        seq_t candidate;
        while (true) {
            candidate = (seq_t)next_k_global++;
            /* d = 2*k - 1  (odd number). */
            seq_t d = candidate * 2 - 1;
            /* floor( d * sqrt(2) ) = floor_sqrt( 2 * d * d ). */
            seq_t t = floor_sqrt((d * d) * 2);
            /* round( d * sqrt(2) / 2 ) = (t + 1) / 2. */
            seq_t round_mid = (t + 1) >> 1;
            /* m = 2*k - 1 + round_mid. */
            seq_t m = candidate * 2 - 1 + round_mid;
            if (is_prime_big(m))
                break;   /* candidate satisfies the definition */
        }
        seq_cache[cache_size++] = candidate;
    }
}

/* Public function that returns the nth term of the sequence
   (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1; callers should not request 0 */
    ensure_terms(n);
    return seq_cache[n - 1];
}
