/*
 * A129119  Numbers of the form 2*p (with p a prime number) such that p^2+4 is
 *          prime.
 * https://oeis.org/A129119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 16, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   grow_seq, is_prime, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129119:
 * Numbers of the form 2*p where p is prime and p^2 + 4 is also prime.
 * Computation: iterate over odd candidates, test primality, and
 * cache the 2*p values.  This is a straightforward prime‑search
 * implementation suitable for n up to ~10 000.  The algorithm uses
 * the provided utilities:
 *   - is_prime(idx_t)      – test 64‑bit primes
 *   - is_prime_big(seq_t) – test arbitrary‑precision primes
 *   - seq_alloc(size_t)   – allocate seq_t[][] array
 *   - seq_realloc(seq_t*,size_t,size_t) – resize the array
 *   - grow_seq(&arr,&cap,need) – utility that resizes to at least *need*
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/grow_seq.h"

/* Return the nth term of A129119, 1‑based indexing. */
seq_t a(idx_t n)
{
    /* cache holds the terms found so far */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* next odd integer to test for primality (starting at 3) */
    static uint64_t next_candidate = 3;

    /* Allocate initial cache if needed */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        uint64_t p = next_candidate;

        if (is_prime((idx_t)p)) {
            /* compute p^2 + 4 in seq_t arithmetic */
            seq_t sq = (seq_t)p * (seq_t)p + (seq_t)4;
            if (is_prime_big(sq)) {
                /* store 2*p */
                if (cache_size >= cache_cap) {
                    grow_seq(&cache, &cache_cap, cache_size + 1);
                }
                cache[cache_size++] = (seq_t)2 * (seq_t)p;
            }
        }

        /* advance to the next odd candidate */
        next_candidate = p + 2;
    }

    return cache[(size_t)n - 1];
}
