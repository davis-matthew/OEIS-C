/*
 * A043145  Numbers k such that 2 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k with 26 or 62 appearing as adjacent digits
 *            in the base‑7 representation of k, while k−1 does not have that
 *            pattern
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: returns true if the base‑7 representation of x contains
   the substring 26 or 62.  */
static bool has_pattern(seq_t x) {
    /* x = 0 or 1 cannot contain a pair of digits */
    if (x < 2) return false;

    const seq_t BASE7 = (seq_t)7;
    seq_t prev = x % BASE7;      /* least significant digit */
    x = x / BASE7;               /* remaining higher‑order digits */

    while (x > 0) {
        seq_t digit = x % BASE7;
        if ((digit == 2 && prev == 6) || (digit == 6 && prev == 2))
            return true;
        prev = digit;
        x = x / BASE7;
    }
    return false;
}

/* Store the sequence as it is generated.  */
static seq_t *seq_terms = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static seq_t last_k      = 0;   /* largest k checked so far */

/* Return the nth term of the sequence (n >= 1).  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* not defined, but safe return */

    /* Grow the cache until we have at least n elements. */
    while (seq_len < n) {
        ++last_k;   /* next integer to test */

        if (has_pattern(last_k) && !has_pattern(last_k - 1)) {
            /* Ensure capacity for the new term. */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_terms = (seq_t *)realloc(seq_terms, seq_cap * sizeof(seq_t));
            }
            seq_terms[seq_len++] = last_k;
        }
    }
    return seq_terms[n - 1];
}
