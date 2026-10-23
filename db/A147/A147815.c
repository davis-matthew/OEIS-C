/*
 * A147815  Triangle t(n, k) = k*n*(prime(n+2) - 2*prime(n+1) + prime(n)) +
 *          prime(n), 0 <= k <= n = 1, 2, 3, ...
 * https://oeis.org/A147815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   a(n) = t(N,K) where N is the first row with * N(N+3)/2 ≥ n, * K
 *            = n − (N−1)(N+2)/2 − 1 (0‑based column inside the row), * t(N,K)
 *            = K*N*(prime(N+2)−2*prime(N+1)+prime(N)) + prime(N)
 * Range:     i8 n <= 42, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* Provides uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1; return 0 for safety */
        return 0;

    /* Find the smallest N such that N(N+3)/2 >= n.
     * Binary search between 1 and n (N cannot exceed n). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Use 128‑bit arithmetic to avoid overflow of mid*(mid+3)  */
        unsigned __int128 val = (unsigned __int128)mid * (mid + 3) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t N = low;                      /* required row */

    /* Elements before row N: sum_{i=1}^{N-1} (i+1) = (N-1)(N+2)/2 */
    unsigned __int128 sumPrev128 = (unsigned __int128)(N - 1) * (N + 2) / 2;
    idx_t  sumPrev = (idx_t)sumPrev128;        /* fits in idx_t */

    /* 0‑based column within the row */
    idx_t K = n - sumPrev - 1;

    /* Retrieve the needed primes */
    uint32_t pN  = get_prime(N);      /* prime(N)   */
    uint32_t pN1 = get_prime(N + 1);  /* prime(N+1) */
    uint32_t pN2 = get_prime(N + 2);  /* prime(N+2) */

    /* Compute the triangle entry */
    seq_t diff  = (seq_t)pN2 - (seq_t)2 * (seq_t)pN1 + (seq_t)pN;
    seq_t result = (seq_t)K * (seq_t)N * diff + (seq_t)pN;
    return result;
}
