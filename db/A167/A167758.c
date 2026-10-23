/*
 * A167758  Numbers k such that d(k) = nonisolated nonprime.
 * https://oeis.org/A167758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   divisor_count_u64, is_prime, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth positive integer k such that
 *             divisor_count(k) is a nonisolated nonprime.
 *             A nonisolated nonprime is a nonprime that is not
 *             surrounded by primes on both sides (1 is considered a
 *             nonisolated nonprime).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t) */
#include "../utils/seq_alloc_array.h"    /* seq_alloc_array */
#include "../utils/seq_realloc.h"        /* seq_realloc */

/* Computes a(n) for any n >= 1.  Calls may be in any order.
 * The sequence is generated lazily and cached in static storage. */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;   /* array holding the sequence */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* current capacity of cache */

    /* Next k to test; starts from 1 */
    static idx_t next_k = 1;

    /* Behaviour for n < 1 is irrelevant; just return 0 */
    if (n < 1) return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t d = (idx_t)divisor_count_u64((uint64_t)next_k);
        bool accept = false;

        if (d <= 1) {
            /* 1 is a nonisolated nonprime by definition */
            accept = true;
        } else if (!is_prime(d)) {
            /* nonprime: check if it is nonisolated */
            if (!(is_prime(d - 1) && is_prime(d + 1))) {
                accept = true;
            }
        }

        if (accept) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* Reallocate using seq_realloc to avoid explicit free */
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
