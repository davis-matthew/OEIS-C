/*
 * A162969  Number of permutations of {1,2,...,n} in which the fixed points
 *          and the non-fixed points alternate.
 * https://oeis.org/A162969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 25, u64 n <= 40, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A162969: Number of permutations of {1,2,…,n} in which
   the fixed points and the non‑fixed points alternate.
   Formula: a(2n)    = 2*d(n)             (n>0)
            a(2n-1)  = d(n-1) + d(n)    (n>0)
   where d(k) = A000166(k) is the number of derangements.
*/

#include <stdint.h>  /* for uint64_t */

static seq_t der[5001];   /* der[k] = d(k) for 0 ≤ k ≤ 5000 */
static idx_t der_nmax = 0;/* highest der index that has been computed */

/* Return A162969(n) */
seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0)
        return 1;

    /* Number of derangements needed: kmax = ceil(n/2) */
    idx_t kmax = (n + 1) / 2;

    /* Ensure der[0]..der[kmax] are computed */
    if (kmax > der_nmax) {
        /* Initialise der[0] and der[1] if they are not set yet */
        if (der_nmax == 0) {
            der[0] = 1;      /* d(0) */
            if (kmax >= 1)   /* we may need d(1) */
                der[1] = 0;  /* d(1) */
            der_nmax = 1;    /* last index computed so far */
        }
        /* Compute remaining values up to kmax */
        for (idx_t i = der_nmax + 1; i <= kmax; ++i) {
            der[i] = (seq_t)(i - 1) * (der[i - 1] + der[i - 2]);
        }
        der_nmax = kmax;
    }

    /* Apply the explicit formula */
    if ((n & 1) == 0) {          /* even n = 2k */
        idx_t k = n / 2;
        return der[k] * 2;
    } else {                     /* odd n = 2k-1 */
        idx_t k = (n + 1) / 2;   /* k = ceil(n/2) = (n+1)/2 */
        return der[k - 1] + der[k];
    }
}
