/*
 * A143541  Triangle read by rows, T(n,k) = 1 if both n and k are prime, 0
 *          otherwise; 1 <= k <= n.
 * https://oeis.org/A143541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if both r and k are prime, where * r =
 *            ceil((sqrt(8*n+1)-1)/2), k = n - r*(r-1)/2; otherwise a(n)=0
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Core function computing A143541 */
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Compute row index r */
    idx_t t = 8 * n + 1;                     /* 8n+1, fits in idx_t for n up to ~2^63 */
    idx_t s = _floor_isqrt_u64(t);           /* floor(sqrt(8n+1)) */
    idx_t r = (s - 1) / 2;                   /* initial approximation */

    /* Ensure r is the smallest integer with r*(r+1)/2 >= n */
    if (r * (r + 1) / 2 < n)
        ++r;

    /* Position k within the row */
    idx_t prev = (r - 1) * r / 2;            /* number of terms before row r */
    idx_t k = n - prev;

    /* Check if both r and k are prime */
    if (is_prime(r) && is_prime(k))
        return (seq_t)1;
    else
        return (seq_t)0;
}
