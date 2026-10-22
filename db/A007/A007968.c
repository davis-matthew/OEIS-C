/*
 * A007968  Type of happy factorization of n.
 * https://oeis.org/A007968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a square; otherwise a(n) = 1 if n = r*s with r
 *            > 1 and r*x^2 - s*y^2 = 1 solvable, else 2. Found by scanning
 *            the convergents p/q of the continued fraction of sqrt(n) over
 *            two periods, using p^2 - n*q^2 = (-1)^(k+1)*Q and checking that
 *            Q | n and Q | p
 * Range:     u8 n <= 300, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   a7968_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a7968_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t a0 = a7968_isqrt(N);
    if (a0 * a0 == N) return (seq_t)0;

    uint64_t m = 0, d = 1, ak = a0;
    uint64_t hprev = 1 % N, h = 0; /* h_{-1}=1, h_{-2}=0, taken mod N */
    uint64_t k = 0;
    int ends = 0;
    while (ends < 2) {
        /* h_k = a_k*h_{k-1} + h_{k-2} (mod N) */
        uint64_t hk = (ak % N * hprev + h) % N;
        h = hprev;
        hprev = hk;
        /* next partial quotient state */
        m = d * ak - m;
        d = (N - m * m) / d;
        /* h_k^2 - N*q_k^2 = (-1)^(k+1) * d */
        int positive = (k % 2 == 1);
        if (!positive && d == 1) return (seq_t)1;           /* n*x^2 - y^2 = 1 */
        if (d > 1 && N % d == 0 && hk % d == 0) {
            if (positive) return (seq_t)1;                  /* r = d > 1 */
            if (d < N) return (seq_t)1;                     /* r = N/d > 1 */
        }
        if (d == 1) ends++;
        ak = (a0 + m) / d;
        k++;
    }
    return (seq_t)2;
}
