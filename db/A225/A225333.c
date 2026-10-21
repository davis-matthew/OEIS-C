/*
 * A225333  Look-and-repeat: similar to look-and-say except frequency is
 *          repeated (seed 2).
 * https://oeis.org/A225333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   get_digits_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A225333 – Look‑and‑repeat with seed 2
 *  a(1) = 2
 *  For n > 1: a(n) is obtained by scanning a(n‑1) from left to right,
 *  grouping equal consecutive digits.  For each run of length k and
 *  digit d, emit the decimal representation of k twice followed by d.
 *  Example: a(2) = 112 → run (2, '1') → 221112.
 *
 *  This implementation builds each term from the previous one and
 *  works for 8, 16, 32, 64‑bit unsigned integers or for arbitrary‑precision
 *  types that support the usual arithmetic operators.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_digits_generic.h"

/* ---------- helpers --------------------------------------------------- */

/* Compute the next term of the sequence from the current term. */
static seq_t next_term(seq_t prev)
{
    uint8_t *digits = NULL;
    size_t len = 0;
    get_digits_generic(prev, &digits, &len);

    if (digits == NULL) return prev;   /* failure – return unchanged term */

    seq_t res = (seq_t)0;

    for (size_t i = 0; i < len; ) {
        /* Identify the run of equal digits. */
        size_t j = i;
        while (j < len && digits[j] == digits[i]) ++j;

        size_t run_len = j - i;
        uint8_t digit  = digits[i];

        /* Convert run_len to decimal digits (least‑significant first). */
        uint8_t runbuf[32];
        size_t rd = 0;
        size_t tmp = run_len;
        do {
            runbuf[rd++] = tmp % 10;
            tmp /= 10;
        } while (tmp > 0);

        /* Append run_len twice. */
        for (int rep = 0; rep < 2; ++rep) {
            for (size_t k = 0; k < rd; ++k) {
                uint8_t d = runbuf[rd - 1 - k];
                res = res * (seq_t)10 + (seq_t)d;
            }
        }

        /* Append the original digit. */
        res = res * (seq_t)10 + (seq_t)digit;

        i = j;
    }

    free(digits);
    return res;
}

/* ---------- main entry ------------------------------------------------ */

/* External linkage required by the framework. */
seq_t a(idx_t n)
{
    /* Undefined for n == 0, but a(0) is not needed. */
    if (n == (idx_t)0) return (seq_t)0;

    if (n == (idx_t)1) return (seq_t)2;

    seq_t cur = (seq_t)2;
    for (idx_t step = (idx_t)2; step <= n; ++step) {
        cur = next_term(cur);
    }

    return cur;
}
