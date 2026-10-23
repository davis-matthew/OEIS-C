/*
 * A153194  Numbers such that the numerator of floor(sqrt(n))/n, when reduced
 *          to its lowest terms, is equal to 3.
 * https://oeis.org/A153194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*g^2 + d*g, where g = floor((n-1)/4) + 1 and d =
 *            {1,2,4,5} for (n-1)%4 = {0,1,2,3} respectively. This yields the
 *            n for which floor(sqrt(n))/n reduces to a fraction whose
 *            numerator is 3
 * Range:     u8 n <= 20, u16 n <= 340, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, avoid underflow */
    idx_t t = n - 1;
    idx_t gidx = t / 4;     /* zero‑based index for g-1 */
    idx_t rem = t % 4;
    seq_t g = (seq_t)(gidx + 1);

    seq_t d;
    switch (rem) {
        case 0: d = (seq_t)1; break;
        case 1: d = (seq_t)2; break;
        case 2: d = (seq_t)4; break;
        default: d = (seq_t)5; break; /* rem == 3 */
    }

    /* compute 9*g*g + d*g */
    seq_t term = (seq_t)9 * g * g + d * g;
    return term;
}
