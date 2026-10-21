/*
 * A097604  a(n) = floor( phi(n)*sqrt(2*n) ) - n.
 * https://oeis.org/A097604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( φ(n) * sqrt(2*n) ) – n (φ = Euler's totient)
 * Range:     u8 n <= 36, u16 n <= 1318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* ---------- compute φ(n) (Euler's totient) ---------- */
    uint64_t nn   = (uint64_t)n;
    uint64_t phi  = nn;
    uint64_t tmp  = nn;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            phi -= phi / p;
        }
    }
    if (tmp > 1) phi -= phi / tmp;  /* remaining prime factor */

    /* ---------- compute floor( φ(n) * sqrt(2*n) ) ---------- */
    /* φ(n) * sqrt(2n)  =  sqrt( (2n) * φ(n)^2 )   (since all are integers) */
    unsigned __int128 product = ((unsigned __int128)2 * nn) * phi * phi;

    /* integer square root of product (floor) */
    unsigned __int128 low  = 0;
    unsigned __int128 high = 1;
    while (high <= product / high)             /* find a high bound  */
        high <<= 1;
    while (high - low > 1) {                    /* binary search */
        unsigned __int128 mid = (low + high) >> 1;
        if (mid <= product / mid)
            low = mid;
        else
            high = mid;
    }
    seq_t sqrt_floor = (seq_t)low;

    /* ---------- final result ---------- */
    seq_t result = sqrt_floor - (seq_t)nn;
    return result;
}
