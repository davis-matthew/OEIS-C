/*
 * A041906  Numerators of continued fraction convergents to sqrt(475).
 * https://oeis.org/A041906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 17, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=21, a(1)=22.
   For n>=1: let c_k be the k-th partial quotient of the continued fraction
   of sqrt(475), which repeats with period 10:
   c = {1, 3, 1, 6, 2, 6, 1, 3, 1, 42}
   Then a(n) = c_{n-1} * a(n-1) + a(n-2). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial two numerators: a(-1) = 1, a(0) = 21 */
    if (n == 0) return (seq_t)21;

    /* Periodic partial quotients of sqrt(475) */
    static const seq_t period[10] = {
        (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)6, (seq_t)2,
        (seq_t)6, (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)42
    };

    seq_t prev2 = (seq_t)1;      /* a(-1) */
    seq_t prev1 = (seq_t)21;     /* a(0) */

    /* Iterate up to the requested index n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = period[(i - 1) % 10];
        seq_t cur = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
