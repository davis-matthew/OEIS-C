/*
 * A135038  Sums of the products of n consecutive quadruples of numbers.
 * https://oeis.org/A135038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} (4k-4)(4k-3)(4k-2)(4k-1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 38, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: compute the sum of the 4‑term products using integer arithmetic. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, but guard against 0 */

    unsigned __int128 N = n;                   /* use 128‑bit to avoid overflow */

    /* sums of powers of k */
    unsigned __int128 S1 = N * (N + 1) / 2;                    /* sum k   */
    unsigned __int128 S2 = N * (N + 1) * (2 * N + 1) / 6;      /* sum k^2 */
    unsigned __int128 tmp = N * (N + 1) / 2;                   /* temp for k^3 */
    unsigned __int128 S3 = tmp * tmp;                          /* sum k^3 */
    unsigned __int128 S4 = N * (N + 1) * (2 * N + 1) *
                           (3 * N * N + 3 * N - 1) / 30;        /* sum k^4 */

    /* closed‑form expression for the sum of products */
    unsigned __int128 res = 256 * S4 - 640 * S3 + 560 * S2 -
                            200 * S1 + 24 * N;

    return (seq_t)res;                     /* cast to the target type */
}
