/*
 * A043113  Numbers k such that 2 and 3 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043113: numbers k such that 2 and 3 occur juxtaposed in the base‑5
   representation of k but not of k‑1.  The function returns the n‑th such
   number (offset 1).  The result is computed by scanning integers, testing
   each for the required pattern, and storing the values in a static cache so
   that the function works efficiently for any call order. */
#include <stdlib.h>
#include <stdbool.h>

static bool has_adjacent_23(seq_t x)
{
    if (x == 0) return false;
    unsigned char digits[128];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (unsigned char)(t % 5);
        t /= 5;
    }
    /* Scan digits from most to least significant. */
    for (size_t i = len; i > 1; --i) {
        if ((digits[i-1] == 2 && digits[i-2] == 3) ||
            (digits[i-1] == 3 && digits[i-2] == 2))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1; undefined below 1 */

    /* Static cache for previously computed terms. */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_cnt = 0;
    static seq_t current = 1;                 /* next integer to test */

    while (seq_cnt < n) {
        /* Find next integer satisfying the pattern. */
        while (!has_adjacent_23(current) || has_adjacent_23(current - 1))
            current++;

        /* Grow the cache if necessary. */
        if (seq_cnt >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* allocation failure */
            seq = tmp;
            seq_cap = new_cap;
        }

        seq[seq_cnt++] = current;
        current++;
    }

    return seq[n-1];
}
