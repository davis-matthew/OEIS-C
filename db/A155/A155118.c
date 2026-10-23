/*
 * A155118  Array T(n,k) read by antidiagonals: the k-th term of the n-th
 *          iterated differences of A140429.
 * https://oeis.org/A155118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by traversing the array T(n,k) by
 *            antidiagonals, where T(n,k) is defined as T(n,k) = 2^n * 3^(k-1)
 *            for k > 0 T(n,0) = (2^n - (-1)^n) / 3 (recurrence from
 *            T(n,0)=2^(n-1)-T(n-1,0)) The index n is converted to diagonal d
 *            and position r along the diagonal, and T(d-r,r) is returned
 * Range:     u8 n <= 32, u16 n <= 87, u32 n <= 272, u64 n <= 943,
 *            u128 n <= 3484, big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline idx_t find_diagonal(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Locate diagonal d and offset r on that diagonal */
    idx_t d = find_diagonal(n);
    __uint128_t start128 = (__uint128_t)d * (d + 1) / 2;
    idx_t r = (idx_t)(n - (idx_t)start128);      /* position inside diagonal */
    idx_t n_val = d - r;                         /* row index of T */
    idx_t k_val = r;                             /* column index of T */

    if (k_val == 0) {
        /* T(n,0) = (2^n - (-1)^n) / 3 */
        seq_t pw = pow2(n_val);
        seq_t numerator = ((n_val & 1) == 0) ? (pw - (seq_t)1) : (pw + (seq_t)1);
        return numerator / (seq_t)3;
    } else {
        return pow2(n_val) * pow3(k_val - 1);
    }
}
