/*
 * A000358  Number of binary necklaces of length n with no subsequence 00,
 *          excluding the necklace "0".
 * https://oeis.org/A000358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(n/d) * L(d), where L(d) =
 *            Fib(d-1)+Fib(d+1) is the Lucas number, L(0) = 2, L(1) = 1, L(k)
 *            = L(k-1) + L(k-2). Each Lucas number is kept as a pair (q, r)
 *            with L = q*n + r and 0 <= r < n, so no intermediate exceeds
 *            about a(n)
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 54, u64 n <= 101,
 *            u128 n <= 195, big any n
 * Helpers:   a358_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a358_totient.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    /* prev = L(k-1), cur = L(k), as (q, r) with value q*N + r */
    seq_t pq = (seq_t)(2 / N);
    uint64_t pr = 2 % N;
    seq_t cq = (seq_t)(1 / N);
    uint64_t cr = 1 % N;

    seq_t Q = 0;
    uint64_t R = 0;

    for (uint64_t k = 1; k <= N; k++) {
        if (N % k == 0) {
            uint64_t ph = a358_totient(N / k);
            unsigned __int128 t = (unsigned __int128)ph * cr;
            Q += (seq_t)ph * cq + (seq_t)(uint64_t)(t / N);
            R += (uint64_t)(t % N);
            if (R >= N) {
                Q += (seq_t)(R / N);
                R %= N;
            }
        }
        if (k == N) break;
        /* next = cur + prev */
        seq_t nq = cq + pq;
        uint64_t nr = cr + pr;
        if (nr >= N) {
            nr -= N;
            nq += 1;
        }
        pq = cq;
        pr = cr;
        cq = nq;
        cr = nr;
    }
    return Q;
}
