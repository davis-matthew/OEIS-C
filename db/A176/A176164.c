/*
 * A176164  Primes p such that (p-2)/7 is not a prime number.
 * https://oeis.org/A176164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that (p-2)/7 is not a prime number.
   The implementation generates successive primes and keeps those
   that do not satisfy the condition.  It uses the provided
   is_prime_big() function for primality testing and a simple
   dynamic array to memoize results.  Previous terms are cached so
   that non‑increasing calls to a(idx_t) work efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Ensure a dynamic array has capacity for at least *cnt elements. */
static void ensure_cap(size_t *cnt, size_t *cap, void **arr, size_t elem_size)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        void *p = realloc(*arr, new_cap * elem_size);
        if (!p) abort();                  /* fatal on allocation failure */
        *arr = p;
        *cap = new_cap;
    }
}

/* Static state for incremental prime generation and sequence storage. */
static seq_t *seq_arr   = NULL;
static idx_t  seq_cnt   = 0;
static idx_t  seq_cap   = 0;
static seq_t  next_candidate = 2;             /* next number to test */

/* Return the n‑th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset safety */

    while (seq_cnt < (size_t)n) {
        /* Test candidate for primality. */
        if (is_prime_big(next_candidate)) {
            bool accept = true;
            /* Only primes congruent to 2 mod 7 can be removed. */
            if ((next_candidate % (seq_t)7) == (seq_t)2) {
                seq_t k = (next_candidate - (seq_t)2) / (seq_t)7;
                if (is_prime_big(k))
                    accept = false;
            }
            if (accept) {
                ensure_cap(&seq_cnt, &seq_cap,
                           (void**)&seq_arr, sizeof(seq_t));
                seq_arr[seq_cnt++] = next_candidate;
            }
        }
        /* Advance to the next integer.  Overflow is unacceptable
           for practical n <= 10000, but we avoid wrapping. */
        next_candidate = next_candidate + (seq_t)1;
    }
    return seq_arr[(size_t)n - 1];
}
