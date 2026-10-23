/*
 * A040574  Continued fraction for sqrt(599).
 * https://oeis.org/A040574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the continued‑fraction expansion of √599. 
   a(0) = ⌊√599⌋ = 24, and a(n) for n≥1 is the n‑th partial quotient. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static const idx_t N = 599;           /* Square‑free part of the continued fraction. */
static bool initialized = false;      /* Has a0 been computed? */
static idx_t a0 = 0;                  /* ⌊√N⌋ – first term of the expansion. */

/* Compute ⌊√N⌋ by a simple integer loop (N = 599). */
static void init_a0(void) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= N) {
        ++r;
    }
    a0 = r;
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) {
        init_a0();
    }
    if (n == 0) {
        return (seq_t)a0;
    }

    /* Algorithm for continued fractions of square roots. */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_term = a0;
    for (idx_t i = 1; i <= n; ++i) {
        m = d * a_term - m;
        d = (N - m * m) / d;
        a_term = (a0 + m) / d;
    }
    return (seq_t)a_term;
}
