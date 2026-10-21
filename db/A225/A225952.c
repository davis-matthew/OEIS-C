/*
 * A225952  Triangle read by rows, giving the even legs of primitive
 *          Pythagorean triangles, with zero entries for non-primitive
 *          triangles.
 * https://oeis.org/A225952
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle: a(n,m) = 2*n*m for n>m>=1,
   gcd(n,m)=1 and n and m of opposite parity;
   otherwise a(n,m)=0.
   The sequence a(n) (n>=2) is this triangle
   flattened row‐wise with offset 2. */
#include <stdint.h>
#include <math.h>

static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;    /* offset is 2; below that is undefined */

    /* t is the 0‑based index inside the flattened triangle */
    idx_t t = n - 2;

    /* Find the row r (n-index) containing this element.
       Total elements up to row r is (r-1)*r/2.            */
    double sq = sqrt((double)1 + 8.0 * (double)t);
    idx_t r = (idx_t)(floor((1.0 + sq) / 2.0)) + 1;

    /* Number of elements before row r */
    idx_t prev = (r - 2) * (r - 1) / 2;

    /* Column index m (1‑based) within row r */
    idx_t m = t - prev + 1;

    /* Parity check: opposite parity required */
    if ( ((r ^ m) & 1) == 0 ) return (seq_t)0;

    /* gcd check */
    if (gcd_uint64(r, m) != 1) return (seq_t)0;

    /* Compute the value: a(n,m) = 2 * n * m */
    return ((seq_t)2) * (seq_t)r * (seq_t)m;
}
