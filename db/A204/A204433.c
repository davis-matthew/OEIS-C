/*
 * A204433  Symmetric matrix: f(i,j) = (2*i + 2*j + 2) mod 3, by
 *          antidiagonals.
 * https://oeis.org/A204433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2 * ((ceil((1 + sqrt(1 + 8*n)) / 2) + 1) % 3)) % 3
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation finds the diagonal index s that satisfies s*(s-1)/2 >= n */
/* and then returns (2 * ((s + 1) % 3)) % 3. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the minimal s >= 2 such that s*(s-1)/2 >= n */
    idx_t low = 2;
    idx_t high = 2;
    /* Grow high until it is large enough to contain n */
    while ((unsigned __int128)high * (high - 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search for the exact s */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t s = low;                /* Diagonal index */
    idx_t mod3 = (s + 1) % 3;     /* (s + 1) mod 3 */
    seq_t val = ((seq_t)2 * (seq_t)mod3) % (seq_t)3;
    return val;
}
