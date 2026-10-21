/*
 * A216627  Triangle read by rows, n>=1, 1<=k<=n, T(n,k) = sum_{c|n,d|k}
 *          lcm(c,d).
 * https://oeis.org/A216627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) with T(n,k) = Σ_{c|n, d|k} lcm(c,d). Row and
 *            column are determined by the triangular index: n lies in row r
 *            where r*(r+1)/2 ≥ n and r*(r-1)/2 < n
 * Range:     u8 n <= 75, u16 n <= 9867, u32 n <= 9151, u64 n <= 9151,
 *            u128 n <= 9151, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Compute column index k */
    unsigned __int128 offset = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = (idx_t)(n - offset);

    /* Generate divisors of r */
    seq_t divr[128];
    size_t cntr = 0;
    for (idx_t i = 1; (unsigned __int128)i * i <= r; ++i) {
        if (r % i == 0) {
            divr[cntr++] = (seq_t)i;
            if (i != r / i) {
                divr[cntr++] = (seq_t)(r / i);
            }
        }
    }

    /* Generate divisors of k */
    seq_t divk[128];
    size_t cntk = 0;
    for (idx_t i = 1; (unsigned __int128)i * i <= k; ++i) {
        if (k % i == 0) {
            divk[cntk++] = (seq_t)i;
            if (i != k / i) {
                divk[cntk++] = (seq_t)(k / i);
            }
        }
    }

    /* Sum lcm over all divisor pairs */
    seq_t res = 0;
    for (size_t i = 0; i < cntr; ++i) {
        for (size_t j = 0; j < cntk; ++j) {
            seq_t g = gcd(divr[i], divk[j]);
            seq_t l = (divr[i] / g) * divk[j];
            res += l;
        }
    }
    return res;
}
