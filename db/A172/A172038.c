/*
 * A172038  Smallest nonnegative integer such that n + (a(n))^2 is a perfect
 *          square, or -1 if no such integer exists.
 * https://oeis.org/A172038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = minimal nonnegative integer m such that n + m^2 is a
 *            perfect square; if no such m exists (i.e. n ≡ 2 (mod 4)), the
 *            value is –1
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for uint64_t */

/* --------------------------------------------------------------- */
/* Integer square‑root for idx_t (unsigned 64‑bit).  Returns floor(sqrt(n)). */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;

    /* Newton‑Raphson iteration – converges very quickly for 64‑bit. */
    idx_t x  = n;
    idx_t y  = (x >> 1) + 1;          /* initial guess */
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the OEIS offset.  Behaviour for n = 0 is
       unspecified and not needed. */
    if ((n & 3U) == 0x02U)          /* n ≡ 2 (mod 4) → no solution */
        return (seq_t)-1;

    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t d1 = sqrt_n; d1 != 0; --d1) {
        if (n % d1 != 0)
            continue;               /* not a divisor */

        idx_t d2 = n / d1;
        if ((d1 & 1U) != (d2 & 1U))
            continue;               /* parity mismatch – not a valid factor pair */

        /* Both d1 and d2 have same parity → (d2 – d1) is even, giving an integer m. */
        seq_t m = (seq_t)((d2 - d1) >> 1);
        return m;
    }

    /* Falling through means no appropriate divisor was found – this can only
       happen for n ≡ 2 (mod 4), already handled above.  Return –1 to satisfy
       the function contract. */
    return (seq_t)-1;
}
