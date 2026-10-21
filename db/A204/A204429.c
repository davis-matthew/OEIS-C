/*
 * A204429  Symmetric matrix: f(i,j)=(2*i + 2*j) mod 3, by antidiagonals.
 * https://oeis.org/A204429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * (( ceil((sqrt(8*n+1)-1)/2 ) + 1 ) mod 3)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    t = minimal integer with t*(t+1)/2 >= n
 *    a(n) = (2*(t+1)) mod 3
 *    Since the value only depends on t, we avoid large arithmetic
 *    by taking the result modulo 3 early.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the smallest t such that t*(t+1)/2 >= n */
    uint64_t low = 0, high = 1;
    unsigned __int128 target = (unsigned __int128)n;

    while ((unsigned __int128)high * (high + 1) / 2 < target) {
        /* Double high, guarding against overflow */
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    while (high > low + 1) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= target)
            high = mid;
        else
            low  = mid;
    }

    uint64_t t = high;                /* minimal t */
    uint64_t t_mod = (t % 3 + 1) % 3;   /* (t+1) mod 3, avoiding overflow */
    seq_t val = (seq_t)((2 * t_mod) % 3);
    return val;
}
