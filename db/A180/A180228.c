/*
 * A180228  Triangular array T(A,B) read by rows: minimal number of steps
 *          required to obtain exactly 2 liters in jug A (irrespective of jug
 *          B), starting with infinite supply of water and two empty jugs with
 *          capacities A and B liters. -1 if not possible. A>=B>=1.
 * https://oeis.org/A180228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   triangular array T(A,B) – minimal number of steps to get exactly
 *            2 liters in jug A (or –1 if impossible). A≥B≥1. Indexed by the
 *            standard OEIS antidiagonal numbering: 1⇒(A=1,B=1), 2⇒(A=2,B=1),
 *            3⇒(A=2,B=2), etc
 * Range:     i8 n <= 2273, i16 n <= 9647, i32 n <= 9647, i64 n <= 9647,
 *            i128 n <= 9647, big any n
 * Helpers:   steps_start_from_A, steps_start_from_B
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/steps_start_from_A.h"
#include "../utils/steps_start_from_B.h"

/* --- local helpers ---------------------------------------------- */

/* integer GCD */
static inline int gcd_int_local(int a, int b) {
    while (b) { int t = b; b = a % b; a = t; }
    return a;
}

/* --- required sequence function ---------------------------------- */

seq_t a(idx_t n) {
    /* --- convert index n to pair (A,B) --------------------------- */
    idx_t A = 1, cumulative = 1;
    while (n > cumulative) {
        ++A;
        cumulative += A;
    }
    idx_t B = n - (cumulative - A);   /* B in range 1..A */

    /* ----- trivial impossibility --------------------------------- */
    if (A < 2) return (seq_t)-1;           /* cannot hold 2 liters */

    /* ----- gcd check --------------------------------------------- */
    int Ai = (int)A, Bi = (int)B;
    int g = gcd_int_local(Ai, Bi);
    if (2 % g != 0) return (seq_t)-1;       /* unreachable */

    /* ----- compute minimal steps from both possible starting jug */
    int stepsA = steps_start_from_A(Ai, Bi, 2);
    int stepsB = steps_start_from_B(Ai, Bi, 2);

    /* --- take the smaller positive value ------------------------ */
    int result = (stepsA < stepsB) ? stepsA : stepsB;
    return (seq_t)result;
}
