/*
 * A157423  Triangle read by rows, T(n,k) = 0 if (n-k+1) is prime, else 1.
 * https://oeis.org/A157423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if (row‑col+1) is prime, else 1; here n is the linear
 *            index in the triangle read by rows, with row r and column c
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   is_prime, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/triangular.h"

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r‑1) < n <= triangular(r). */
    idx_t lo = 1, hi = 1;
    while (triangular(hi) < n) hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    idx_t prev = triangular(r - 1);
    idx_t c = n - prev;           /* column in the row */
    idx_t m = r - c + 1;          /* number to test for primality */

    return is_prime(m) ? (seq_t)0 : (seq_t)1;
}
