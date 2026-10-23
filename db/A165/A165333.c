/*
 * A165333  Numbers that eventually reach the fixed point 370 under "x -> sum
 *          of cubes of digits of x" (see A055012).
 * https://oeis.org/A165333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165333: numbers that eventually reach the fixed point 370
   under the map x → sum of cubes of decimal digits.
   This implementation uses Floyd’s cycle‑finding algorithm to avoid
   infinite loops for numbers that fall into other cycles. */

#include <stdbool.h>
#include <stdlib.h>

/* The table of digit cubes. */
static const int cube[10] = {0, 1, 8, 27, 64, 125, 216, 343, 512, 729};

/* Sum of the cubes of decimal digits of a number. */
static seq_t sum_cubes_digits(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        x = x / 10;
        sum += (seq_t)cube[d];
    }
    return sum;
}

/* One iteration of the map. */
static seq_t next_value(seq_t x)
{
    return sum_cubes_digits(x);
}

/* Determines whether a starting value eventually reaches 370.
   Uses Floyd's cycle detection to terminate even if the orbit is a
   cycle that does not contain 370. */
static bool reaches370(seq_t x)
{
    static const seq_t TARGET = (seq_t)370;

    if (x == TARGET) return true;

    seq_t tortoise = next_value(x);
    seq_t hare     = next_value(next_value(x));

    /* Detect a cycle; stop when tortoise == hare. */
    while (tortoise != hare) {
        if (tortoise == TARGET || hare == TARGET) return true;
        tortoise = next_value(tortoise);
        hare     = next_value(next_value(hare));
    }

    /* Cycle found: check if it contains the target. */
    return tortoise == TARGET;
}

/* Dynamic array holding the discovered terms. */
static seq_t *terms     = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t  last_checked = 1;  /* next integer to test            */

/* Ensure capacity of the terms buffer. */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined index → 0   */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        seq_t cand = (seq_t)last_checked;
        if (reaches370(cand)) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = cand;
        }
        ++last_checked;
    }
    return terms[n - 1];
}
