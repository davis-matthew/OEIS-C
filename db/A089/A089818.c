/*
 * A089818  T(n,k) = number of subsets of {1,..., n} containing exactly k
 *          primes, triangle read by rows, 0<=k<n.
 * https://oeis.org/A089818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   OEIS A089818 – Triangle entries T(n,k) = binomial(pi(n),k) *
 *            2^(n-pi(n)). The sequence a(n) is the triangle read by rows:
 *            a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), a(4)=T(3,0), …. For a
 *            given flattened index N the corresponding row r is the smallest
 *            integer with r(r+1)/2 >= N; the column k is N - (r...
 * Range:     u8 n <= 46, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom, compute_pi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/compute_pi.h"   /* seq_t compute_pi(idx_t n); */
#include "../utils/binom.h"       /* seq_t binom(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

seq_t a(idx_t N)
{
    if (N == 0)               /* offset for safety */
        return 0;

    /* find smallest r with r(r+1)/2 >= N using binary search */
    idx_t low = 1, high = N;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                                 /* row number */
    unsigned __int128 tri_prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = (idx_t)(N - tri_prev - 1);           /* column index */

    idx_t pi = (idx_t)compute_pi(r);               /* number of primes ≤ r */
    if (k > pi)
        return 0;                                  /* column exceeds number of primes */

    seq_t bin = binom(pi, k);                      /* C(pi, k) */
    seq_t pw  = pow2(r - pi);                      /* 2^(r-pi) */
    return bin * pw;                               /* final value */
}
