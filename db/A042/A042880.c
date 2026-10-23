/*
 * A042880  Numerators of continued fraction convergents to sqrt(972).
 * https://oeis.org/A042880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=31, a(1)=156.
   For n≥2, let c[i] be the periodic continued‑fraction coefficients of √972
   of period 20: 5,1,1,1,7,6,1,3,1,14,1,3,1,6,7,1,1,1,5,62.
   Then a(n) = c[(n-1) mod 20] * a(n-1) + a(n-2). */

/* Necessary headers for C11 and C++17 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Return the nth term of OEIS sequence A042880. */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)31;
    if (n == 1) return (seq_t)156;

    /* Periodic continued‑fraction coefficients of √972.  */
    static const int coeff[20] = {
        5, 1, 1, 1, 7, 6, 1, 3, 1, 14,
        1, 3, 1, 6, 7, 1, 1, 1, 5, 62
    };

    seq_t a_prev2 = (seq_t)31; /* a(0) */
    seq_t a_prev1 = (seq_t)156;/* a(1) */
    seq_t a_curr;

    /* Compute the terms from 2 up to n. */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        size_t idx = (i - 1) % 20;          /* index into coeff array */
        a_curr = (seq_t)coeff[idx] * a_prev1 + a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
