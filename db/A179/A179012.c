/*
 * A179012  Primes that are the sum of three consecutive composite odd
 *          numbers.
 * https://oeis.org/A179012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 709, u32 n <= 941, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th prime that is the sum of three consecutive composite odd numbers.
 * Computation proceeds by scanning odd numbers, collecting composite odds in a sliding window,
 * summing them, and testing the sum for primality. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* state kept between calls to build the sequence incrementally */
static seq_t c1 = 0, c2 = 0, c3 = 0;
static idx_t seen_composite = 0;
static idx_t next_odd = 3;  /* the next odd number to examine */

/* Ensure at least `target` terms have been generated. */
static void ensure_cache(idx_t target)
{
    /* fast exit if already have enough terms */
    if (cache_len >= (size_t)target)
        return;

    while (cache_len < (size_t)target) {
        /* find next odd composite number */
        while (true) {
            if (!is_prime_big((seq_t)next_odd))
                break;          /* next_odd is composite */
            next_odd += 2;       /* skip to the next odd */
        }

        /* process this composite odd */
        seen_composite++;
        if (seen_composite == 1) {
            c1 = next_odd;
        } else if (seen_composite == 2) {
            c2 = next_odd;
        } else {
            c3 = next_odd;
            seq_t sum = c1 + c2 + c3;
            if (is_prime_big(sum)) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = sum;
            }
            /* shift window for next triple */
            c1 = c2;
            c2 = c3;
        }

        /* prepare for next odd number */
        next_odd += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1, but return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
