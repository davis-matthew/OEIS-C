/*
 * A162620  Triangle read by rows in which row n lists n consecutive natural
 *          numbers A000027, starting with A000040(n)-n+1.
 * https://oeis.org/A162620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = prime(r) - r + (n - (r-1) * r / 2), where r is the
 *            smallest integer with r*(r+1)/2 ≥ n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Find the row r such that T(r-1) < n ≤ T(r), where T(k)=k*(k+1)/2 */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Compute position within row r */
    unsigned __int128 t_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t T_prev = (idx_t)t_prev;                /* T_prev = (r-1)*r/2 */
    seq_t pos = (seq_t)(n - T_prev);             /* 1‑based position in the row */

    /* Get the r-th prime and compute the value */
    seq_t prime_r = (seq_t)get_prime(r);
    seq_t result = prime_r - (seq_t)r + pos;
    return result;
}
