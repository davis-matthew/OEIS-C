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
 * Validated: standard, pass, 84 terms through n = 9926 (2026-11-01)
 * Helpers:   is_palindrome_base, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/num_divisors.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
        seq_t d = num_divisors(candidate);          /* number of divisors of candidate */
        if (is_palindrome_base(d, 10)) {                       /* check if d is a palindrome */
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
