/*
 * A049702  Array T read by diagonals; T(i,j)=number of directions determined
 *          by 2 lattice points in [ 0,i ]x[ 0,j ].
 * https://oeis.org/A049702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = H(i) + V(j) + 2·∑_{x=1..i}∑_{y=1..j} [gcd(x,y)=1] where
 *            (i,j) are the coordinates corresponding to the index n: let s be
 *            the largest integer with s·(s+1)/2 ≤ n, offset = s·(s+1)/2, i =
 *            n – offset, j = s – i
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Euclidean gcd for uint64_t values. */
static inline uint64_t u64_gcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find diagonal index s such that s·(s+1)/2 ≤ n < (s+1)·(s+2)/2 */
    double ds = sqrt(8.0 * n + 1.0);
    idx_t s = (idx_t)((ds - 1.0) / 2.0);

    /* Adjust in case of round‑off errors */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    idx_t offset = s * (s + 1) / 2;
    idx_t i = n - offset;          /* 0 ≤ i ≤ s */
    idx_t j = s - i;               /* 0 ≤ j ≤ s */

    /* Horizontal and vertical directions (counted once each if they exist) */
    seq_t val = 0;
    if (i >= 1) val += 1;          /* horizontal line (1,0) */
    if (j >= 1) val += 1;          /* vertical line (0,1) */

    /* Count primitive vectors with 1 ≤ x ≤ i and 1 ≤ y ≤ j.
       For each such vector we get two directions (±). */
    uint64_t cop = 0;              /* will fit comfortably into 64 bits */
    for (uint64_t x = 1; x <= (uint64_t)i; ++x) {
        for (uint64_t y = 1; y <= (uint64_t)j; ++y) {
            if (u64_gcd(x, y) == 1) ++cop;
        }
    }
    val += (seq_t)(2 * cop);
    return val;
}
