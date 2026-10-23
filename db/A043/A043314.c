/*
 * A043314  a(n)=A033008(n)/11.
 * https://oeis.org/A043314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑100 representation * uses
 *            only digits < 10 and has no adjacent equal digits. * This
 *            implements OEIS A043314
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow9_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow9_idx.h"

seq_t a(idx_t n)
{
    /* n is 1‑based. undefined for n < 1. */
    idx_t target = n;

    /* Determine the number of base‑100 digits of a(target). */
    idx_t len = 1;
    idx_t cumulative = 0;
    idx_t countLen = pow9_idx(len);  /* 9^len  (count of length‑len numbers) */

    while (cumulative + countLen < target) {
        cumulative += countLen;
        ++len;
        countLen = pow9_idx(len);
    }

    /* Offset within the block of length‑len numbers (0‑based). */
    idx_t offset = target - cumulative - 1;

    /* Build the number digit by digit. */
    seq_t value = 0;
    idx_t prev = 0;                /* previous base‑100 digit */

    /* First digit: 1‑9, each block has pow9(len-1) numbers. */
    idx_t per_first = pow9_idx(len - 1);
    idx_t div = offset / per_first;
    idx_t first_digit = 1 + div;   /* in 1..9 */
    value = (seq_t)first_digit;
    prev = first_digit;
    offset %= per_first;

    /* Remaining digits.  Each has 9 choices (0‑9 except prev). */
    for (idx_t pos = 2; pos <= len; ++pos) {
        value *= 100;             /* shift one base‑100 digit */

        idx_t per_rest = pow9_idx(len - pos);
        idx_t idx_digit = offset / per_rest;
        offset %= per_rest;

        /* Find the idx_digit‑th allowed digit (0‑9 except prev). */
        idx_t d = 0;
        for (idx_t cand = 0; cand <= 9; ++cand) {
            if (cand == prev)
                continue;
            if (idx_digit == 0) {
                d = cand;
                break;
            }
            --idx_digit;
        }

        value += (seq_t)d;
        prev = d;
    }

    return value;
}
