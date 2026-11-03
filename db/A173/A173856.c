/*
 * A173856  Expansion of 10/9 in base phi.
 * https://oeis.org/A173856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n-th digit (from the left) in the base‑φ expansion
 *            of 10/9. The expansion is obtained by the greedy β‑expansion
 *            algorithm: starting with x₀ = 10/9, for k ≥ 0 set dₖ = ⌊xₖ⌋ (0
 *            or 1) and next value xₖ₊₁ = φ·(xₖ – dₖ)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper that decides whether the digit is 1 or 0 from (p,q) representing
   x = (p + q·φ) / 9  with φ = (1+√5)/2.  The relation x ≥ 1 is equivalent to
   (2p + q) ≥ 18  or, if that fails and q > 0, to  (18 – (2p+q))² ≤ 5·q².  */
static int digit_from_pq(int64_t p, int64_t q)
{
    int64_t c = 2 * p + q;
    if (c >= 18)
        return 1;
    if (q <= 0)
        return 0;
    int64_t t = 18 - c;               /* 0 < t ≤ 17 */
    /* All intermediate values fit in 64‑bit (t² ≤ 289, q² is small).  */
    if (t * t <= 5 * q * q)
        return 1;
    return 0;
}

/* Main sequence entry point.  */
seq_t a(idx_t n)
{
    /* Representation of the current value xk as (p + q·φ)/9.  */
    int64_t p = 10;   /* 10/9  = (10 + 0·φ)/9  */
    int64_t q = 0;

    for (idx_t i = 1; i <= n; ++i) {
        int d = digit_from_pq(p, q);           /* current digit */

        if (i == n)
            return (seq_t)d;                   /* return the desired digit */

        /* Update (p,q) for the next iteration:
           p' = b = q
           q' = a + b with a = p - 9·d                               */
        int64_t a = p - 9 * d;
        int64_t b = q;
        int64_t new_p = b;
        int64_t new_q = a + b;
        p = new_p;
        q = new_q;
    }
    /* If n == 0 the definition is irrelevant; return 0.  */
    return 0;
}
