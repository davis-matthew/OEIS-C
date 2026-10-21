/*
 * A052688  Expansion of e.g.f. x/((1-x)*(1-x^3)).
 * https://oeis.org/A052688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*ceil(n/3), a(0) = 0 Recurrence in OEIS: (n+2)*a(n+3) =
 *            (n+3)*a(n+2) + (n+2)*(n+3)*a(n+1) + (n+1)*(n+2)*(n+3)^2*a(n),
 *            a(0)=0,a(1)=1,a(2)=2 The closed form simplifies to
 *            a(n)=n!*ceil(n/3) for all n≥1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0u) return (seq_t)0;

    /* Compute n! iteratively. The multiplication uses the built‑in seq_t ops,
       which are exact for arbitrary-precision types and safe for uint128 on
       the 64‑bit platforms. */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* ceil(n/3) = (n+2)/3 for integer division */
    seq_t mult = (seq_t)((n + 2u) / 3u);

    return fact * mult;
}
