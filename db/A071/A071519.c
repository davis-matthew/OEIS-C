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
 * Helpers:   grow_terms, pandigital9_no0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_terms.h"
#include "../utils/pandigital9_no0.h"

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
