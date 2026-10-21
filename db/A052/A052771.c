/*
 * A052771  Expansion of e.g.f. x^3*exp(x)^2.
 * https://oeis.org/A052771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 50, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*(n-1)*(n-2) * 2^(n-3) for n >= 3,
 * a(0) = a(1) = a(2) = 0.
 *
 * The implementation uses only integer arithmetic and bit shifts,
 * so it works for 8‑, 16‑, 32‑, 64‑bit and arbitrary‑precision seq_t
 * types without overflow for values that fit in the corresponding type.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n < 3 gives 0 */
    if (n < 3)
        return 0;

    /* For n>=3 we can apply the closed‑form expression.
       It equals: n*(n-1)*(n-2) * 2^(n-3) .
       The product is always divisible by 8, so the shift by (n-3)
       is safe and avoids any division. */
    seq_t prod = (seq_t)n * (n - 1) * (n - 2);
    idx_t shift = n - 3;
    /* Shift left by (n-3) bits, i.e. multiply by 2^(n-3). */
    while (shift > 0) {
        prod <<= 1;
        --shift;
    }
    return prod;
}
