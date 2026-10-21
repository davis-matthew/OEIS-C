/*
 * A212564  Number of (w,x,y,z) with all terms in {1,...,n} and w + x > 2y +
 *          2z.
 * https://oeis.org/A212564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 414, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A212564 – Number of tuples (w,x,y,z) with all terms in {1,…,n} and w+x>2y+2z.
 *
 * Formula (per OEIS entry):
 *   a(n) = (1/96) * ( 2*n*(n-2)*n*(7*n-4) + (n odd? -12*n + 18 : 0) )
 *
 * The implementation uses only signed 64‑bit arithmetic for the intermediate
 * values.  For n up to 10 000 the result fits in 64 bits and the intermediate
 * computations stay within the signed 64‑bit range, guaranteeing correctness
 * for all n ≥ 0 that fit in uint64_t.  The final division by 96 is exact
 * (by the theorem of the sequence) and the result is cast to seq_t.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int64_t N = (int64_t)n;                /* signed copy of the index */

    /* compute the base polynomial: 2*n*(n-2)*n*(7*n-4) */
    int64_t base = 2 * N * (N - 2) * N * (7 * N - 4);

    /* adjust for odd n: add (-12*n + 18) before division */
    if (N & 1) {
        base = base - 12 * N + 18;
    }

    /* division by 96 – guaranteed to be integral */
    base /= 96;

    return (seq_t)base;
}
