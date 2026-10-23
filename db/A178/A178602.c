/*
 * A178602  Expansion of the polynomial (1+x^3)*(1+x^11)*(1+x^15)*(1+x^23).
 * https://oeis.org/A178602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the coefficient of x^n in
 *            (1+x^3)*(1+x^11)*(1+x^15)*(1+x^23)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It equals the number of subsets of {3,11,15,23} whose exponents sum to n. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The four exponents that can appear in the product. */
    static const idx_t exponents[4] = {3, 11, 15, 23};

    /* Maximum possible degree: 3+11+15+23 = 52. */
    static const idx_t max_degree = 52;

    /* For n larger than the maximum degree, the coefficient is zero. */
    if (n > max_degree)
        return (seq_t)0;

    seq_t count = 0;
    /* Iterate over all 2^4 subsets of the exponent set. */
    for (idx_t mask = 0; mask < ((idx_t)1 << 4); ++mask)
    {
        idx_t sum = 0;
        /* Add the selected exponents to the sum. */
        sum += (mask & 1) ? exponents[0] : 0;
        sum += (mask & 2) ? exponents[1] : 0;
        sum += (mask & 4) ? exponents[2] : 0;
        sum += (mask & 8) ? exponents[3] : 0;

        if (sum == n)
            ++count;
    }

    return count;
}
