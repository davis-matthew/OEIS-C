/*
 * A059045  Square array T(n,k) read by antidiagonals where T(0,k) = 0 and
 *          T(n,k) = 1 + 2k + 3k^2 + ... + n*k^(n-1).
 * https://oeis.org/A059045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(d - r, r) where d*(d+1)/2 ≤ n < (d+1)*(d+2)/2, r = n -
 *            d*(d+1)/2, and T(n,k) = 1 + 2k + 3k^2 + … + n*k^(n‑1)
 * Range:     u8 n <= 37, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal index d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t low  = 0;
    idx_t high = 1;
    while ((__uint128_t)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (high - low > 1) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    idx_t d   = low;
    idx_t prev = d * (d + 1) / 2;   /* number of elements before diagonal d */
    idx_t r   = n - prev;           /* position inside the diagonal */
    idx_t n_prime = d - r;          /* the n of T(n,k) */
    idx_t k_prime = r;              /* the k of T(n,k) */

    if (n_prime == 0) return 0;     /* T(0,k) = 0 */

    seq_t sum = 0;
    seq_t pow = 1;                  /* k^(i-1) */
    seq_t k   = (seq_t)k_prime;

    for (idx_t i = 1; i <= n_prime; ++i) {
        sum += (seq_t)i * pow;
        pow *= k;
    }
    return sum;
}
