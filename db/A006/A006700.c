/*
 * A006700  T(n,3,1), where T(n,k,s) with 0<=s<n is the number of sequences
 *          a_1,...,a_(k*n+s) of integers 0,1,...,n such that the 'bumped'
 *          sequence b_1,...,b_(k*n+s) has exactly s n's, where b_i=a_i + j
 *          (mod n+1) with minimal j>=0 such that b_0,...,b_i contain at most
 *          k elements equal to b_i.
 * https://oeis.org/A006700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = P(3n+1), where P(m) is the number of sequences of length
 *            m whose sorted entries x_(j) satisfy x_(j) < u_j, with u_j =
 *            floor((j-1)/3)+1. With circular bumping, spot n holding exactly
 *            one car means the other spots are full and nothing wraps around.
 *            That reduces to this linear condition. The...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   alloc_zero, release, u_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u_of.h"

static seq_t pow_seq(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t N = 3 * n + 1;
    seq_t *P = alloc_zero(N + 1);
    seq_t *R = alloc_zero(N + 1);
    P[0] = 1;
    R[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        for (idx_t j = m; j >= 1; j--) R[j] += R[j - 1];
        idx_t x = u_of(m);
        seq_t s = pow_seq((seq_t)x, m);
        idx_t curd = (idx_t)-1;
        seq_t p = 0;
        for (idx_t jj = m; jj >= 1; jj--) {
            idx_t j = jj - 1;
            idx_t d = x - u_of(j + 1);
            idx_t e = m - j;
            if (d != curd) {
                p = pow_seq((seq_t)d, e);
                curd = d;
            } else {
                p *= (seq_t)d;
            }
            s -= R[j] * P[j] * p;
        }
        P[m] = s;
    }
    seq_t res = P[N];
    release(P, N + 1);
    release(R, N + 1);
    return res;
}
