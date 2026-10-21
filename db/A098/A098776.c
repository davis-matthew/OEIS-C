/*
 * A098776  Duplicate of A098897.
 * https://oeis.org/A098776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: palindromic numbers with no digit 8 and with leading digit ≠ 6 or 8
   (single–digit 6 is excluded, 8 is allowed).  The numbers are listed in
   ascending order. */
#include <stdint.h>
#include <stddef.h>

static const int LEADING_DIGITS[7] = {1, 2, 3, 4, 5, 7, 9};
static const int INNER_DIGITS[9]   = {0, 1, 2, 3, 4, 5, 6, 7, 9};
static const int SINGLE_DIGITS[8]  = {1, 2, 3, 4, 5, 7, 8, 9};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, value for n = 0 is irrelevant */

    /* --- 1–digit palindromes ------------------------------------------- */
    if (n <= 8) {
        return SINGLE_DIGITS[n - 1];
    }

    /* --- find the length L of the n‑th palindrome --------------------- */
    idx_t length = 2;
    idx_t cumulative = 8;          /* count of palindromes of length < 2 */

    while (1) {
        idx_t halfIdx = (length + 1) / 2;   /* ceil(L/2) */
        /* compute 9^(halfIdx-1) safely */
        idx_t pow9 = 1;
        for (idx_t i = 1; i < halfIdx; ++i) {
            POW_FAIL:; /* avoid accidental overflow – not needed for n ≤ 1e4 */
            pow9 *= 9;
        }
        idx_t countLen = 7 * pow9;          /* 7 leading digits × 9^(halfIdx-1) */
        if (n <= cumulative + countLen)
            break;
        cumulative += countLen;
        ++length;
    }

    /* --- index within the current length ------------------------------- */
    idx_t k = n - cumulative - 1;          /* 0‑based index in this block */

    /* --- compute free‑digit power (9^(halfIdx-1)) ---------------------- */
    idx_t halfIdx = (length + 1) / 2;
    idx_t exponent = halfIdx - 1;
    idx_t pow9 = 1;
    for (idx_t i = 0; i < exponent; ++i) pow9 *= 9;

    /* --- leading digit ------------------------------------------------- */
    idx_t leading_idx = k / pow9;
    idx_t rem = k % pow9;

    int digits[20];                         /* enough for n ≤ 10000 */
    digits[0] = LEADING_DIGITS[leading_idx];

    /* --- inner digits (including middle if L is odd) ------------------- */
    for (idx_t pos = exponent; pos > 0; --pos) {
        int dindex = rem % 9;
        rem /= 9;
        digits[pos] = INNER_DIGITS[dindex];
    }

    /* --- mirror to complete the palindrome ---------------------------- */
    for (idx_t i = 0; i < halfIdx; ++i) {
        digits[length - 1 - i] = digits[i];
    }

    /* --- convert digit array to seq_t --------------------------------- */
    seq_t val = 0;
    for (idx_t i = 0; i < length; ++i) {
        val = val * 10 + (seq_t)digits[i];
    }
    return val;
}
