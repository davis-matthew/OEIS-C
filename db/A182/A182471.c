/*
 * A182471  If n = Product (p_j^k_j), j=1 to m, then a(n) = Floor( Sum (p_j +
 *          k_j) / 2m ) (a(1) = 0 by convention).
 * https://oeis.org/A182471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_j (p_j + k_j) / (2m) )
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1u)                 /* sequence convention */
        return 0u;

    idx_t tmp = n;
    unsigned __int128 sum = 0;
    unsigned m = 0;

    /* factor 2 separately to speed up */
    if ((tmp & 1u) == 0u) {
        unsigned k = 0;
        while ((tmp & 1u) == 0u) {
            tmp >>= 1;
            k++;
        }
        sum += (unsigned __int128)2 + k;
        m++;
    }

    for (idx_t p = 3; ; p += 2) {
        if ((unsigned __int128)p * p > tmp) break;
        if (tmp % p == 0u) {
            unsigned k = 0;
            while (tmp % p == 0u) {
                tmp /= p;
                k++;
            }
            sum += (unsigned __int128)p + k;
            m++;
        }
    }

    if (tmp > 1u) {                /* remaining prime factor */
        sum += (unsigned __int128)tmp + 1;
        m++;
    }

    unsigned __int128 denom = (unsigned __int128)(2u * m);
    unsigned __int128 result = sum / denom;

    return (seq_t)result;
}
