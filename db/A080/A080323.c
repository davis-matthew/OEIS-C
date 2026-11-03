/*
 * A080323  a(n) = mu(n)^n, where mu is the Moebius function (A008683).
 * https://oeis.org/A080323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mu(n)^n
 * Range:     i8 n <= 16385, i16 n <= 16385, i32 n <= 16385, i64 n <= 16385,
 *            i128 n <= 16385, big any n
 * Validated: standard, pass, 116 terms through n = 9945 (2026-11-01)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the supplied Möbius routine, which returns a signed seq_t value. */
#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* below offset: undefined, safe as 0 */

    seq_t m = mobius(n);                  /* μ(n) ∈ { -1, 0, 1 } */

    if (m == 0) return (seq_t)0;          /* μ(n) = 0  → 0^n = 0 */
    if (m == 1) return (seq_t)1;          /* μ(n) = 1  → 1^n = 1 */

    /* μ(n) = -1  → (-1)^n = 1 if n even, -1 if n odd */
    return (n & 1ULL) ? (seq_t)-1 : (seq_t)1;
}
