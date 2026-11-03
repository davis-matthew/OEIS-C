/*
 * A048322  Numbers whose base-11 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test whether the base‑11 representation of `value` has strictly decreasing run
   lengths.  The function works for any seq_t supported by the build (u8, u16,
   u32, u64, __int128, or an arbitrary‑precision integer class). */
static bool dec_run_ok(seq_t value) {
    /* Count the number of base‑11 digits */
    seq_t tmp = value;
    idx_t len = 0;
    while (tmp) {
        tmp /= 11;
        len++;
    }
    if (len == 0)          /* value is 0 */
        return true;

    /* For small lengths avoid dynamic allocation */
    if (len <= 128) {
        unsigned char digits[128];
        tmp = value;
        for (idx_t i = len; i > 0; --i) {
            digits[i - 1] = (unsigned char)(tmp % 11);
            tmp /= 11;
        }
        idx_t last_len = len + 1;   /* greater than any possible run length */
        idx_t run_len = 1;
        unsigned char cur_digit = digits[0];
        for (idx_t i = 1; i < len; ++i) {
            if (digits[i] == cur_digit) {
                run_len++;
            } else {
                if (run_len >= last_len)
                    return false;
                last_len = run_len;
                cur_digit = digits[i];
                run_len = 1;
            }
        }
        return run_len < last_len;
    } else {
        /* Fallback for very long numbers: allocate the required buffer. */
        unsigned char *digits = (unsigned char *)malloc((size_t)len);
        if (!digits)
            return false;   /* unlikely for the expected n range */
        tmp = value;
        for (idx_t i = len; i > 0; --i) {
            digits[i - 1] = (unsigned char)(tmp % 11);
            tmp /= 11;
        }
        idx_t last_len = len + 1;
        idx_t run_len = 1;
        unsigned char cur_digit = digits[0];
        for (idx_t i = 1; i < len; ++i) {
            if (digits[i] == cur_digit) {
                run_len++;
            } else {
                if (run_len >= last_len) {
                    free(digits);
                    return false;
                }
                last_len = run_len;
                cur_digit = digits[i];
                run_len = 1;
            }
        }
        free(digits);
        return run_len < last_len;
    }
}

/* Sequence A048322: numbers whose base‑11 expansions have strictly decreasing run
   lengths.  The function searches integers sequentially and returns the n‑th term. */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cur = 0;
    while (1) {
        if (dec_run_ok(cur)) {
            if (count == n)
                return cur;
            ++count;
        }
        ++cur;
    }
}
