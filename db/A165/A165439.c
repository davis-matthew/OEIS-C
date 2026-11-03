/*
 * A165439  Primes p such that 3+2*(sum of digits of p) is also a prime.
 * https://oeis.org/A165439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 960 (2026-11-03)
 * Helpers:   digit_sum, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS sequence A165439: a prime p is in the
   sequence iff 3 + 2·digit_sum(p) is also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

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
