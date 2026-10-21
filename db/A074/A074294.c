/*
 * A074294  Integers 1 to 2*k followed by integers 1 to 2*k + 2 and so on.
 * https://oeis.org/A074294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = n - 2*binomial(floor(1/2 + sqrt(n)), 2) Implemented using
 *            integer arithmetic only
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for idx_t (uint64_t). */
static idx_t isqrt(idx_t n) {
    if (n < 2) return n;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* before the offset, safe guard */
    /* Compute k = floor(1/2 + sqrt(n)) */
    idx_t s = isqrt(n);                /* floor(sqrt(n)) */
    /* Test whether sqrt(n) has fractional part >= 0.5 */
    unsigned __int128 four_n = (unsigned __int128)4 * n;
    unsigned __int128 t      = (unsigned __int128)(2 * s + 1) * (2 * s + 1);
    idx_t k = s;
    if (four_n >= t) k = s + 1;        /* sqrt(n) >= s + 0.5 */
    /* Compute a(n) = n - k*(k-1) */
    idx_t sub = k * (k - 1);           /* fits in 64 bits */
    idx_t res = n - sub;
    return (seq_t)res;
}
