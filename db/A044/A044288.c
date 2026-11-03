/*
 * A044288  Numbers n such that string 4,1 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑9 representation of n contains the
   substring “41” but the base‑9 representation of n‑1 does not. */

/* Implementation uses a simple brute‑force search with memoisation.
   Because the density of qualifying numbers is about 1/81, the 10,000th term
   is well below one million, so the routine is comfortably fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: does the base‑9 representation of x contain the substring 41? */
static bool has41(seq_t x)
{
    /* At most 128 base‑9 digits are enough for every value that will be
       encountered in the tested range (the sequence values never exceed a few
       million).  For arbitrary‑precision seq_t the loop terminates at 0. */
    uint8_t digits[128];
    int pos = 0;

    /* Extract base‑9 digits (least‑significant first). */
    while (x != (seq_t)0) {
        digits[pos++] = (uint8_t)(x % (seq_t)9);
        x = x / (seq_t)9;
    }
    if (pos < 2) return false;

    /* Scan from most‐significant to least‐significant to find the substring
       41. */
    for (int i = pos - 1; i >= 1; --i) {
        if (digits[i] == 4 && digits[i - 1] == 1)
            return true;
    }
    return false;
}

/* Main function: a(n) as defined by OEIS A044288. */
seq_t a(idx_t n)
{
    /* Static memoisation storage. */
    static seq_t *vals = NULL;   /* array of found numbers */
    static size_t count = 0;     /* number of elements stored */
    static size_t capacity = 0;  /* allocated capacity */
    static seq_t last_checked = (seq_t)0;   /* last candidate examined */

    if (n == 0)   /* the sequence is undefined for n < 1; return 0 */
        return (seq_t)0;

    /* Grow storage if necessary. */
    if (vals == NULL) {
        capacity = 16;
        vals = mem_new(seq_t, capacity);
    }

    /* Find elements until we have at least n of them. */
    while (count < n) {
        /* Advance candidate. */
        last_checked = last_checked + (seq_t)1;
        seq_t cand = last_checked;

        /* Check the property. */
        if (has41(cand) && !has41(cand - (seq_t)1)) {
            /* Store the value. */
            if (count == capacity) {
                capacity *= 2;
                vals = mem_resize(seq_t, vals, capacity);
            }
            vals[count++] = cand;
        }
    }

    /* Return the requested element. */
    return vals[n - 1];
}
