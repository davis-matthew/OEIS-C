/*
 * A209898  T(n,k)=Number of (n+1)X(k+1) 0..2 arrays with every 2X2 subblock
 *          having one or four distinct clockwise edge differences.
 * https://oeis.org/A209898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the values T(i,j) (with i,j >= 1) of A209898 in
 *            anti‑diagonal order: For each integer d >= 2, the pairs (i,j)
 *            with i+j=d are visited in the order i = 1 .. d-1 (j = d-i).
 *            T(n,k) = z(n) + z(k) - 5, where z(x) = 3 * 2^(x/2 + 1) if x is
 *            even, 2^((x-1)/2 + 3) if x is odd
 * Range:     u8 n <= 55, u16 n <= 351, u32 n <= 1512, u64 n <= 1512,
 *            u128 n <= 1512, big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 // should not be requested (offset is 1)

    /* find diagonal d such that C(d-1) < n <= C(d)
       C(d) = d*(d-1)/2  (number of terms up to diagonal d)      */
    idx_t d = 2;
    seq_t nseq = (seq_t)n;
    while (((seq_t)d * (d - 1)) / 2 < nseq) {
        ++d;
    }

    seq_t prev = ((seq_t)(d - 1) * (d - 2)) / 2;   /* terms before diagonal d */
    idx_t pos = (idx_t)(nseq - prev);              /* position within the diagonal (1‑based) */

    idx_t i = pos;            /* first index of the pair */
    idx_t j = d - pos;        /* second index of the pair */

    /* compute z(i) */
    seq_t zi;
    if ((i & 1) == 0) { /* even */
        zi = mul_small(pow2(i/2 + 1), 3);
    } else {            /* odd */
        zi = pow2((i - 1)/2 + 3);
    }

    /* compute z(j) */
    seq_t zj;
    if ((j & 1) == 0) {
        zj = mul_small(pow2(j/2 + 1), 3);
    } else {
        zj = pow2((j - 1)/2 + 3);
    }

    /* T(i,j) = z(i)+z(j)-5 */
    return zi + zj - (seq_t)5;
}
