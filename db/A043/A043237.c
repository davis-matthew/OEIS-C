/*
 * A043237  Numbers k such that 2 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043237: a(n) is the n-th positive integer k such that the decimal representation
   of k contains the substring "27" or "72", and the integer k−1 does not contain either
   substring. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Test whether x contains "27" or "72" as a decimal substring. */
static bool has_27_or_72(seq_t x)
{
    if (x < (seq_t)10) return false;
    seq_t last = x % (seq_t)10;
    x /= (seq_t)10;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)10;
        if ((cur == (seq_t)2 && last == (seq_t)7) ||
            (cur == (seq_t)7 && last == (seq_t)2))
            return true;
        last = cur;
        x /= (seq_t)10;
    }
    return false;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset starts at 1; behaviour for 0 is irrelevant. */

    static seq_t *terms = NULL;    /* Array of already found terms. */
    static size_t terms_len = 0;   /* Number of terms stored. */
    static size_t terms_cap = 0;   /* Current capacity of terms array. */
    static seq_t next_k = 1;       /* Next integer to test. */

    while (terms_len < (size_t)n) {
        seq_t k = next_k++;
        if (has_27_or_72(k)) {
            seq_t prev = k - (seq_t)1;
            if (!has_27_or_72(prev)) {          /* Ensure k-1 lacks the substring. */
                if (terms_len == terms_cap) {   /* Grow the array if needed. */
                    size_t new_cap = terms_cap ? terms_cap << 1 : 8;
                    terms = mem_resize(seq_t, terms, new_cap);
                    terms_cap = new_cap;
                }
                terms[terms_len++] = k;
            }
        }
    }
    return terms[n - 1];
}
