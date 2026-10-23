/*
 * A132897  Expansion of 1/(1-9x*c(10x)), where c(x) is the g.f. of A000108.
 * https://oeis.org/A132897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Expansion of 1/(1-9x*c(10x)), where c(x) is the generating
 * function of Catalan numbers (A000108).  The sequence satisfies
 *        a(0) = 1,
 *        a(1) = 9,
 *        a(n) = g(n) – 81·a(n‑1)   for n ≥ 2,
 * with g(1) = 90 and for n ≥ 2
 *        g(n) = g(n‑1) · (20·(2n‑3)) / n .
 *
 * The recurrence follows from the closed–form generating function
 *     A(x) = 20/(11 + 9√(1−40x))
 * and guarantees that all intermediate values are integers.
 * This implementation uses only the allowed arithmetic operators and
 * works with every supported seq_t type (fixed‑width or arbitrary
 * precision).  No dynamic allocation or external utilities are used.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* g corresponds to the coefficient g(n) described above.
     * For n = 1 we have g = g(1) = 90, and a_prev = a(1). */
    seq_t g      = (seq_t)90;
    seq_t a_prev = (seq_t)9;

    /* Iterate for n ≥ 2.  All arithmetic uses the over‑loaded
     * operators for seq_t (or the underlying 64‑bit unsigned type). */
    for (idx_t k = 2; k <= n; ++k) {
        /* factor = 20·(2k−3).  Use seq_t on the right‑hand side to avoid
         * any integer overflow on the native type. */
        seq_t factor = (seq_t)20 * ((seq_t)2 * k - 3);

        /* Update g:  g = g * factor / k   (exact division). */
        g = g * factor / k;

        /* Compute a(k):  a(k) = g – 81·a(k‑1). */
        seq_t a_k = g - (seq_t)81 * a_prev;
        a_prev = a_k;
    }

    /* a_prev now holds a(n). */
    return a_prev;
}
