/*
 * A000210  A Beatty sequence: floor(n*(e-1)).
 * https://oeis.org/A000210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(e-1)), computed exactly using integers only:
 *            with T_k = n*k!*Sum_{j>=k} 1/j!, we have T_k = n + T_{k+1}/(k+1)
 *            and a(n) = floor(T_1). floor(T_K) is fixed once floor(n/(K+1)) =
 *            floor((n-1)/K)
 * Range:     u8 n <= 148, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t K = 1;
    uint64_t f;
    uint64_t k;

    if (N == 0)
        return (seq_t)0;

    /* T_K = n + tail, with n/(K+1) < tail < n/K, so
       floor(tail) lies in [floor(n/(K+1)), floor((n-1)/K)]. */
    while (N / (K + 1) != (N - 1) / K)
        K++;

    f = N + N / (K + 1);          /* floor(T_K) */
    for (k = K; k > 1; k--)       /* floor(T_{k-1}) = n + floor(floor(T_k)/k) */
        f = N + f / k;

    return (seq_t)f;
}
