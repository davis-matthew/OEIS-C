/*
 * A047999  Sierpiński's [Sierpinski's] triangle (or gasket): triangle, read
 *          by rows, formed by reading Pascal's triangle (A007318) mod 2.
 * https://oeis.org/A047999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = binomial(r,k) mod 2, where * r = ⌊(√(1+8n) − 1)/2⌋ (the
 *            row number), * k = n − r*(r+1)/2 (column index within row). * By
 *            Lucas' theorem, binomial(r,k) mod 2 = 1 iff * (k & ~r) == 0 else
 *            0. This implements the Sierpinski triangle * read by rows
 * Range:     u8 n <= 9209, u16 n <= 9209, u32 n <= 9209, u64 n <= 9209,
 *            u128 n <= 9209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns floor(√x) for x up to 2^128-1.  */
static inline uint64_t isqrt128(unsigned __int128 x) {
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 35;          /* > √(2^67)  */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;    /* ceil division */
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Compute r = floor((√(1+8n)−1)/2) */
    unsigned __int128 x = (unsigned __int128)1 + (unsigned __int128)8 * n;
    uint64_t m = isqrt128(x);
    idx_t r = (idx_t)((m - 1) >> 1);            /* r fits in idx_t */

    /* Compute k = n − r*(r+1)/2 */
    idx_t triangular = (idx_t)(((unsigned __int128)r * (r + 1)) >> 1);
    idx_t k = n - triangular;

    /* Return 1 if (k & ~r) == 0 else 0, cast to seq_t. */
    return (seq_t)(((k & (~r)) == 0) ? 1 : 0);
}
