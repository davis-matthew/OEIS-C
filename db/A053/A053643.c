/*
 * A053643  a(n) = ceiling(binomial(n,6)/n).
 * https://oeis.org/A053643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(n,6)/n) = ceil(binomial(n-1,5)/6)
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 317, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 the binomial term is zero */
    if (n < 6)
        return 0;

    /* Compute C(n-1,5) exactly using 128‑bit arithmetic to avoid
       overflow in intermediate multiplication.  The algorithm
       prod = prod * (N - i + 1) / i keeps the intermediate value
       integral at every step. */
    __int128 prod = 1;
    idx_t N = n - 1;          /* N = n-1  */
    for (idx_t i = 1; i <= 5; ++i) {
        prod *= (N - i + 1);  /* multiply by the next numerator */
        prod /= i;            /* divide by the corresponding denominator */
    }

    /* Now prod == C(n-1,5).  Ceil division by 6: (prod + 5) / 6. */
    __int128 result = (prod + 5) / 6;
    return (seq_t)result;
}
