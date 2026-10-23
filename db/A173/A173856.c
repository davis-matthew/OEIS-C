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
 * Helpers:   digit_from_pq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_from_pq.h"

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
