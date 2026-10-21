/*
 * A070747  a(n) = signum(sin(n)), where signum=A057427.
 * https://oeis.org/A070747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = signum(sin(n))
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: a(0)=0; for n>0, a(n)=(-1)^floor(n/π) */
/* This implementation computes floor(n/π) using integer arithmetic only. */

#include <stdint.h>
#include <stddef.h>

/* Uses 355/113 as a rational lower bound for π (exactly 113/355 < 1/π).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sin(0)=0 */

    /* Use 128‑bit arithmetic to avoid overflow.  */
    __int128 N = n;            /* (idx_t) → (__int128) */
    const __int128 A = 355;    /* numerator of the upper bound π ≈ 355/113 */
    const __int128 B = 113;    /* denominator */

    /* First approximation of floor(n/π) using 113/355 < 1/π */
    __int128 m = (N * B) / A;  /* floor(n * 113 / 355)  */

    /* Tighten the estimate: increase m while (m+1)·π ≤ n, 
       and decrease it while m·π > n.  Multiplication by π is
       performed via the rational bound A/B.  */
    while ((m + 1) * A <= N * B)
        ++m;
    while (m * A > N * B)
        --m;

    /* Sign is 1 for even m, –1 for odd m.  */
    seq_t sign = (seq_t)((m & 1) ? -1 : 1);
    return sign;
}
