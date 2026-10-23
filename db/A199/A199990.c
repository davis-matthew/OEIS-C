/*
 * A199990  Numbers with digital product = 10.
 * https://oeis.org/A199990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   construct_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199990: numbers with one 2, one 5 and any number of 1s.
   The numbers are listed in increasing numeric order.
   The algorithm enumerates the numbers length‑by‑length, and for a
   fixed length L it generates all patterns by
   – choosing the position i of the leftmost special digit (from L‑2 down to 0),
   – choosing the digit d1 at i (first 2 then 5),
   – choosing the position j of the other digit in descending order.
   This order is exactly the numeric order for all such numbers.
   The routine works for idx_t up to the usual limit and for any
   seq_t that can hold the value.  It requires only basic integer
   arithmetic and the standard <stdbool.h> header.   */

#include <stdbool.h>    /* for bool */
#include "../utils/construct_number.h"

seq_t a(idx_t n) {
    if (n < 1) {
        return 0;   /* undefined for n < 1 */
    }

    /* Step 1: find the length L of the required number. */
    idx_t cum = 0;          /* cumulative count of numbers with lengths < L */
    idx_t L = 2;            /* minimal length is 2 (2 and 5) */
    while (true) {
        idx_t cnt_len = L * (L - 1);   /* count of numbers with length L */
        if (cum + cnt_len >= n) {
            break;
        }
        cum += cnt_len;
        ++L;
    }

    /* Step 2: locate the position inside numbers of length L. */
    idx_t k = n - cum;                 /* 1‑based index within this length */
    idx_t i = L - 2;                   /* start from largest leftmost position */

    /* Find the leftmost position i that contains the first special digit. */
    while (true) {
        idx_t span = L - 1 - i;            /* choices for the other digit */
        idx_t block = 2 * span;            /* two blocks: leading 2 or 5 */
        if (k <= block) {
            break;
        }
        k -= block;
        --i;
    }

    /* Within this block, decide whether the leftmost digit is 2 or 5. */
    idx_t span = L - 1 - i;
    seq_t left_digit = (k <= span) ? 2 : 5;
    idx_t offset_within_block = (k <= span) ? k : k - span;

    /* The rightmost position is chosen in descending order from L-1 to i+1. */
    idx_t right_pos = L - offset_within_block;   /* offset_within_block is 1‑based */

    /* Step 3: construct and return the number. */
    return construct_number(L, i, right_pos, left_digit);
}
