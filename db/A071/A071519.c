/*
 * A071519  Numbers whose square is a zeroless pandigital number (i.e., use
 *          the digits 1 through 9 once).
 * https://oeis.org/A071519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn, base
 * Formula:   a(n) is the n‑th positive integer whose square is a 9‑digit
 *            pandigital * number using the digits 1 through 9 exactly once
 *            (no 0)
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: full, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Resize the dynamic array that holds the pre‑computed terms. */
static void grow_terms(seq_t **arr, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 1 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, (*cap * sizeof(seq_t)) / sizeof(seq_t));
    }
}

/* Test whether a 9‑digit number `v` contains each digit 1–9 exactly once.
 * The routine uses only operations available on `seq_t`.               */
static bool pandigital9_no0(seq_t v)
{
    uint16_t mask = 0;
    while (v > 0) {
        seq_t rem = v % 10;
        if (rem == 0) return false;

        /* Convert the digit to an int by direct comparison with literals. */
        int digit;
        if (rem == 1)      digit = 1;
        else if (rem == 2) digit = 2;
        else if (rem == 3) digit = 3;
        else if (rem == 4) digit = 4;
        else if (rem == 5) digit = 5;
        else if (rem == 6) digit = 6;
        else if (rem == 7) digit = 7;
        else if (rem == 8) digit = 8;
        else if (rem == 9) digit = 9;
        else               return false;  /* digit > 9 */

        mask |= 1u << digit;
        v = v / 10;
    }
    return mask == 0x3FE;  /* bits 1–9 all set */
}

/* Cache of the sequence values. */
static seq_t *cached_terms = NULL;
static size_t cached_len   = 0;
static size_t cached_cap   = 0;
static bool   initialized  = false;

/* Pre‑compute all terms up to the largest integer with a 9‑digit square. */
static void init_sequence(void)
{
    const idx_t MAX_N = 31426;   /* floor(sqrt(987654321)) */
    for (idx_t i = 1; i <= MAX_N; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        if (pandigital9_no0(sq)) {
            grow_terms(&cached_terms, &cached_len, &cached_cap);
            cached_terms[cached_len++] = (seq_t)i;
        }
    }
    initialized = true;
}

/* Required function for the build system. */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if (n == 0 || (size_t)(n - 1) >= cached_len)
        return (seq_t)0;  /* index out of range */
    return cached_terms[n - 1];
}
