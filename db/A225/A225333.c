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
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   none
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

/* Extract the decimal digits of x into an array `digits` in
 * most‑significant‑digit first order.  On return, *len holds
 * the number of digits and *digits points to a newly malloc‑ed array.
 * The caller must free(*digits).
 */
static void get_digits_generic(seq_t x, uint8_t **digits, size_t *len)
{
    /* Handle the special case x == 0 (not used here but kept for completeness). */
    if (x == (seq_t)0) {
        *len = 1;
        *digits = (uint8_t*)malloc(sizeof(uint8_t));
        if (*digits) (*digits)[0] = 0;
        return;
    }

    /* Count digits. */
    size_t count = 0;
    seq_t t = x;
    while (t != (seq_t)0) {
        t = t / (seq_t)10;
        ++count;
    }

    *len = count;
    *digits = (uint8_t*)malloc(count * sizeof(uint8_t));
    if (*digits == NULL) return;              /* out of memory – caller will handle */

    /* Store digits in reverse order then reverse them into MSB-first order. */
    t = x;
    while (t != (seq_t)0) {
        uint8_t d = (uint8_t)(t % (seq_t)10);
        (*digits)[--count] = d;
        t = t / (seq_t)10;
    }
}

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
