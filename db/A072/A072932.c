/*
 * A072932  a(n) is the least k such that floor( (1+1/k)^n ) = floor(
 *          (1+1/n)^k ).
 * https://oeis.org/A072932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{k >= 1 | floor((1+1/k)^n) = floor((1+1/n)^k)}
 * Range:     u8 n <= 280, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: uses logarithms to compare the two floors; because the
   right‑hand side is always 1 or 2, only small exponentiations are needed. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Pre‑compute constants in long double for higher precision */
    const long double ln2 = logl(2.0L);
    const long double ln3 = logl(3.0L);
    const long double ln1pn = log1pl(1.0L / n);   /* log(1+1/n) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute f(k) = n * log(1 + 1/k) */
        const long double f = n * log1pl(1.0L / k);

        /* Determine the left floor value: only 1 or 2 can match the right side */
        int mL;
        if (f < ln2)
            mL = 1;
        else if (f < ln3)
            mL = 2;
        else   /* f >= ln(3) => floor >= 3, cannot match */
            continue;

        /* Compute g(k) = k * log(1 + 1/n) */
        const long double g = k * ln1pn;

        /* Right floor: 1 if g < ln2 else 2 (since g < ln( (1+1/n)^n ) < ln(e) < ln3) */
        int mR = (g < ln2) ? 1 : 2;

        if (mL == mR)
            return (seq_t)k;
    }

    /* Fallback: for completeness (should never happen) */
    return (seq_t)n;
}
