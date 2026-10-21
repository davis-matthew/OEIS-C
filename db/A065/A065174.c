/*
 * A065174  Permutation of Z, folded to N, corresponding to the site swap
 *          pattern ...242824202428242... (A065176).
 * https://oeis.org/A065174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the permutation of Z defined by * sigma(z) = z +
 *            d(|z|), * where d(m) = 0 for m = 0, * 2 for odd m, * 4 if v2(m)
 *            = 1, * 8 << (v2(m) - 2) for v2(m) ≥ 2. * The permutation on Z is
 *            then folded to N by the order * 0,1,-1,2,-2,3,-3,…, and a(n) is
 *            the image of n under this * folding. Th...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n (1‑based) to the corresponding integer z in Z. */
    int64_t z;
    if (n == 1) {
        z = 0;
    } else if (n & 1) {                /* odd n (>=3) */
        z = -((int64_t)(n - 1) / 2);
    } else {                            /* even n */
        z =  (int64_t)(n / 2);
    }

    /* Compute increment d(|z|). */
    int64_t absz = z >= 0 ? z : -z;
    seq_t inc;
    if (absz == 0) {
        inc = 0;
    } else if ((absz & 1LL) == 1LL) {   /* odd */
        inc = 2;
    } else {
        unsigned int v = 0;
        uint64_t t = (uint64_t)absz;
        while ((t & 1ULL) == 0ULL) {    /* count trailing zeros */
            t >>= 1;
            ++v;
        }
        if (v == 1) {                   /* v2 = 1 */
            inc = 4;
        } else {                        /* v2 >= 2 */
            inc = (seq_t)(8ULL << (v - 2));
        }
    }

    /* Apply the permutation. */
    int64_t zprime = z + (int64_t)inc;

    /* Map back to natural numbers using the folding order. */
    int64_t out;
    if (zprime == 0) {
        out = 1;
    } else if (zprime > 0) {
        out = 2 * zprime;
    } else {                          /* zprime < 0 */
        out = 2 * (-zprime) + 1;
    }

    return (seq_t)out;
}
