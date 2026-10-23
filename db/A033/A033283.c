/*
 * A033283  Numbers whose maximal base-10 run length is 2.
 * https://oeis.org/A033283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose maximal base‑10 run length is 2.
   The function returns the n‑th number (1‑based) that
   contains at least one pair of consecutive equal digits
   and no triple or longer equal digits.  It is computed
   by brute‑force testing each integer in increasing order. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_digits.h"

/* Return true if the decimal representation of x has a
   maximal run length equal to 2: at least one run of
   exactly two identical consecutive digits and no run
   of length 3 or more.  The order of digits given by
   get_digits (most or least significant first) does
   not matter, because run lengths are invariant under
   reversal. */
static bool has_exact_run2(seq_t x)
{
    /* 0 and single‑digit numbers cannot have a run of 2 */
    if (x < 10)
        return false;

    uint8_t digits[128];
    size_t len = 0;
    get_digits(x, digits, &len);

    if (len < 2)
        return false;

    bool has_two = false;
    unsigned run = 1;          /* current run length */

    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == digits[i - 1]) {
            ++run;
            if (run >= 3)          /* triple or longer found */
                return false;
        } else {
            if (run == 2)
                has_two = true;
            run = 1;
        }
    }
    if (run == 2)
        has_two = true;

    return has_two;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for zero index; arbitrary choice */

    idx_t found = 0;
    seq_t cand = 11;        /* smallest number with a run of length 2 */

    while (true) {
        if (has_exact_run2(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
}
