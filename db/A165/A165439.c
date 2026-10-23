/*
 * A165439  Primes p such that 3+2*(sum of digits of p) is also a prime.
 * https://oeis.org/A165439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, grow_seq_array, is_prime, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS sequence A165439: a prime p is in the
   sequence iff 3 + 2·digit_sum(p) is also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t); */
#include "../utils/digit_sum.h"      /* seq_t digit_sum(seq_t); */
#include "../utils/grow_seq_array.h" /* void grow_seq_array(seq_t **, size_t *, size_t); */
#include "../utils/seq_alloc_array.h"/* seq_t *seq_alloc_array(size_t); */

/* Return the nth term of A165439 (1‑based index). */
seq_t a(idx_t n)
{
    /* Cache of terms that have already been found. */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cap   = 0;      /* allocated capacity of cache */
    static size_t len   = 0;      /* number of terms cached so far */
    static idx_t  next  = 2;      /* next integer to test */

    /* Initialise cache on the first call. */
    if (cache == NULL) {
        cap   = 16;
        cache = seq_alloc_array(cap);
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        /* Find the next prime that satisfies the condition. */
        for (idx_t cand = next; ; ++cand) {
            /* Skip even numbers > 2. */
            if (cand > 2 && (cand & 1U) == 0)
                continue;

            if (!is_prime(cand))
                continue;

            seq_t s   = digit_sum((seq_t)cand);
            seq_t tmp = 3 + 2 * s;

            if (is_prime((idx_t)tmp)) {
                /* Grow cache if necessary. */
                if (len >= cap)
                    grow_seq_array(&cache, &cap, len + 1);

                cache[len++] = (seq_t)cand;
                next = cand + 1;
                break;      /* found the next term, now check the outer loop */
            }
        }
    }

    return cache[n - 1];          /* 1‑based index → 0‑based array */
}
