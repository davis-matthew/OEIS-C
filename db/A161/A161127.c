/*
 * A161127  Triangle read by rows: T(n,k) is the number of fixed-point-free
 *          involutions of {1,2,...,2n} having k descents (n>=1; 1<=k<2n-1).
 * https://oeis.org/A161127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * 2n T(n,k) = [k(k+1)+2n-2]T(n-1,k) +
 *            2[(k-1)(2n-k-1)+1]T(n-1,k-1) * + [(2n-k)(2n-k+1)+2n-2]T(n-1,k-2)
 *            * Initial value: T(1,1)=1. * The sequence a(n) is the Flattened
 *            triangle: * a(n) = T(r,k) where r = ceil(sqrt(n)) and k =
 *            n-(r-1)^2
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer floor square root for unsigned 64‑bit values */
static idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for non‑positive n */

    /* Determine the row r such that (r-1)^2 < n <= r^2 */
    idx_t r = _floor_isqrt_u64(n);
    if (r * r < n)
        r++;

    idx_t k = n - (r - 1) * (r - 1);  /* 1‑based index inside row r */

    if (r == 1)        /* row 1 has only T(1,1)=1 */
        return (seq_t)1;

    /* Allocate space for the previous and current rows */
    idx_t prev_size = 1;
    seq_t *prev = (seq_t *)malloc(prev_size * sizeof(seq_t));
    if (!prev) return (seq_t)0;
    prev[0] = (seq_t)1;               /* T(1,1) */

    seq_t *curr = NULL;

    for (idx_t cur_n = 2; cur_n <= r; ++cur_n) {
        idx_t cur_size = 2 * cur_n - 1;
        curr = (seq_t *)malloc(cur_size * sizeof(seq_t));
        if (!curr) {
            free(prev);
            return (seq_t)0;
        }

        seq_t denom = 2 * (seq_t)cur_n;

        for (idx_t kp = 1; kp <= cur_size; ++kp) {
            seq_t term1 = 0, term2 = 0, term3 = 0;
            seq_t K = (seq_t)kp;

            /* term1: uses T(prev, kp) */
            if (kp <= prev_size) {
                seq_t coef1 = K * (K + 1) + 2 * (seq_t)cur_n - 2;
                term1 = coef1 * prev[kp - 1];
            }

            /* term2: uses T(prev, kp-1) */
            if (kp >= 2 && (kp - 1) <= prev_size) {
                seq_t coef2 = 2 *
                              ((K - 1) * ((2 * (seq_t)cur_n - K - 1)) + 1);
                term2 = coef2 * prev[kp - 2];
            }

            /* term3: uses T(prev, kp-2) */
            if (kp >= 3 && (kp - 2) <= prev_size) {
                seq_t coef3 =
                    ((2 * (seq_t)cur_n - K) * (2 * (seq_t)cur_n - K + 1) +
                     2 * (seq_t)cur_n - 2);
                term3 = coef3 * prev[kp - 3];
            }

            curr[kp - 1] = (term1 + term2 + term3) / denom;
        }

        free(prev);
        prev = curr;
        prev_size = cur_size;
    }

    seq_t result = prev[k - 1];
    free(prev);
    return result;
}
