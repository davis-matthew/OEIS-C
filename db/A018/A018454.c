/*
 * A018454  Divisors of 435.
 * https://oeis.org/A018454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th divisor of 435 in ascending order (1‑based) */
#include <stdint.h>
/* no other headers are required; seq_t and idx_t are defined by the build */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* undefined for n < 1 */

    /* Prime factorization of 435: 3^1 * 5^1 * 29^1 */
    static const int    num_factors = 3;
    static const uint64_t pf[3] = {3, 5, 29};
    static const int    exp[3] = {1, 1, 1};

    /* There are at most 2^3 = 8 divisors. */
    seq_t divs[8];
    int    count = 0;

    /* Enumerate all exponent combinations and compute the product. */
    for (int e0 = 0; e0 <= exp[0]; ++e0) {
        seq_t v0 = 1;
        for (int i0 = 0; i0 < e0; ++i0) v0 *= pf[0];

        for (int e1 = 0; e1 <= exp[1]; ++e1) {
            seq_t v1 = v0;
            for (int i1 = 0; i1 < e1; ++i1) v1 *= pf[1];

            for (int e2 = 0; e2 <= exp[2]; ++e2) {
                seq_t v2 = v1;
                for (int i2 = 0; i2 < e2; ++i2) v2 *= pf[2];
                divs[count++] = v2;
            }
        }
    }

    /* Sort the small fixed array in ascending order (bubble sort). */
    for (int i = 0; i < count - 1; ++i)
        for (int j = i + 1; j < count; ++j)
            if (divs[i] > divs[j]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    /* Return the requested divisor or 0 if n is out of range. */
    if (n > (idx_t)count)
        return 0;
    return divs[(int)(n - 1)];
}
