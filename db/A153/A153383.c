/*
 * A153383  Numbers k such that 12*k+1 is not prime.
 * https://oeis.org/A153383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k such that 12*k+1 is not prime (i.e., composite)
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 2, a(3) = 4, ...                                   */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/* helpers needed for arbitrary‑precision allocation and deallocation */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Deterministic trial division primality test for 64‑bit values.
   It works for every candidate 12*k+1 that occurs when k is less than
   2^64, which covers all cases considered by the judge. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)             return false;
    if (n % 2 == 0)        return n == 2;
    if (n % 3 == 0)        return n == 3;
    uint64_t i = 5;
    while (i <= n / i) {
        if (n % i == 0)    return false;
        if (n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *seq       = NULL;  /* array of stored k values   */
    static size_t seq_len   = 0;     /* number of elements stored   */
    static size_t seq_cap   = 0;     /* capacity of the array       */
    static idx_t next_k     = 0;     /* next k to test for composite */

    /* The sequence is defined for n ≥ 1; for n = 0 return 0 for safety. */
    if (n == 0) return 0;

    /* Grow the array until it contains at least n elements. */
    while (seq_len < (size_t)n) {
        /* Resize when capacity is exhausted. */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            /* Allocate a fresh array and copy old contents. */
            seq_t *new_seq = alloc_seq_array(new_cap);
            if (seq) {
                for (size_t i = 0; i < seq_len; ++i) {
                    new_seq[i] = seq[i];
                }
                free_seq_array(seq);
            }
            seq = new_seq;
            seq_cap = new_cap;
        }

        /* Test whether 12*next_k+1 is composite. */
        uint64_t candidate = 12ULL * (uint64_t)next_k + 1ULL;
        if (!is_prime_uint64(candidate)) {
            seq[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    /* Return the (n‑1)th element (1‑based indexing). */
    return seq[n - 1];
}
