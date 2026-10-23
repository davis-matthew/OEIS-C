/*
 * A180925  Natural numbers with palindromic number of divisors.
 * https://oeis.org/A180925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th positive integer whose number of divisors is a
 *            palindrome in base 10
 * Range:     u8 n <= 221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_palindrome, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_palindrome.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache of the sequence */
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t candidate = 1;

    if (!initialized) {
        seq_cap = 16;
        seq = seq_alloc(seq_cap);  /* allocates and value‑initialises seq_t array */
        seq_len = 0;
        candidate = 1;
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        seq_t d = divisor_count_u64(candidate);          /* number of divisors of candidate */
        if (is_palindrome(d, 10)) {                       /* check if d is a palindrome */
            if (seq_len == seq_cap) {                     /* grow the cache */
                size_t old_cap = seq_cap;
                seq_cap <<= 1;
                seq = seq_realloc(seq, old_cap, seq_cap); /* returns a new array, copies old elements */
            }
            seq[seq_len++] = candidate;                  /* store the valid integer */
        }
        ++candidate;                                      /* next integer to test */
    }

    return seq[n - 1];                                      /* zero‑based array index */
}
