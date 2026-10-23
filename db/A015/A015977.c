/*
 * A015977  Two iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth number that needs exactly two reverse-and-add
   iterations to reach a palindrome in base 10. */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* Reverse a number in decimal */
static seq_t rev_decimal(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

/* Static storage for the sequence */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t candidate = 0;  /* next number to test */

/*
 * a(n) returns the nth term (0‑based) of A015977.
 * The function builds the sequence lazily and caches the found terms.
 */
seq_t a(idx_t n) {
    /* grow until we have at least n+1 terms */
    while (seq_len <= (size_t)n) {
        seq_t rev1 = rev_decimal(candidate);
        seq_t m = candidate + rev1;

        /* First iteration did not produce a palindrome? */
        if (m != rev_decimal(m)) {
            seq_t rev2 = rev_decimal(m);
            seq_t p = m + rev2;
            /* palindrome after the second iteration */
            if (p == rev_decimal(p)) {
                /* grow the array if necessary */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 128;
                    seq = seq_realloc(seq, seq_len, seq_cap);
                }
                seq[seq_len++] = candidate;
            }
        }

        ++candidate;
    }

    return seq[(size_t)n];
}
