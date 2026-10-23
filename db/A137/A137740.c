/*
 * A137740  Number of different strings of length n+5 obtained from "123...n"
 *          by iteratively duplicating any substring.
 * https://oeis.org/A137740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+4)*(n^2+3*n-8)*(n^2+23*n+150))/120 + 4 for n>4
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 4) {
        switch ((int)n) {
            case 1: return 1;
            case 2: return 32;
            case 3: return 138;
            case 4: return 348;
            default: return 0; /* unreachable */
        }
    }

    seq_t nn = n;                      /* treat index as seq_t once */
    seq_t A = nn + 4;
    seq_t B = nn * nn + 3 * nn - 8;
    seq_t C = nn * nn + 23 * nn + 150;

    /* Reduce the product by 120 without overflow.
       120 = 2^3 * 3 * 5.  We divide each factor out across A, B, C. */
    int denoms[5] = {2, 2, 2, 3, 5};
    for (int i = 0; i < 5; ++i) {
        seq_t d = denoms[i];
        seq_t g = gcd(A, d);
        if (g > 1) { A /= g; d /= g; }
        if (d > 1) {
            g = gcd(B, d);
            if (g > 1) { B /= g; d /= g; }
        }
        if (d > 1) {
            g = gcd(C, d);
            if (g > 1) { C /= g; d /= g; }
        }
        /* after this d should be 1 */
    }

    /* Multiply the remaining factors in increasing order to avoid overflow. */
    seq_t vals[3] = {A, B, C};
    if (vals[0] > vals[1]) { seq_t t = vals[0]; vals[0] = vals[1]; vals[1] = t; }
    if (vals[0] > vals[2]) { seq_t t = vals[0]; vals[0] = vals[2]; vals[2] = t; }
    if (vals[1] > vals[2]) { seq_t t = vals[1]; vals[1] = vals[2]; vals[2] = t; }

    seq_t prod = vals[0] * vals[1];
    prod = prod * vals[2];
    prod += 4;
    return prod;
}
