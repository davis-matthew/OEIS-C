/*
 * A006914  Theta series of laminated lattice LAMBDA_12^{max}.
 * https://oeis.org/A006914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (A^3 - 3*A^2*B + 12*A*B^2 - 4*B^3), with A =
 *            theta_3(q)^4, B = theta_2(q)^4/2, i.e. Theta = theta_3^12 -
 *            (3/2) theta_3^8 theta_2^4 + 3 theta_3^4 theta_2^8 - (1/2)
 *            theta_2^12. [q^k] A = 8*Sum_{d|k, 4 !| d} d; [q^k] B =
 *            8*sigma(k) for odd k, 0 for even k; [q^k] A^2 = r_8(k) =
 *            16*s(k),...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 3905,
 *            u128 n <= 9090, big any n
 * Helpers:   alloc_zero, release, s_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/s_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t len = n + 1;
    seq_t *sig = alloc_zero(len);
    seq_t *r4s = alloc_zero(len);
    seq_t *s3 = alloc_zero(len);
    for (idx_t d = 1; d <= n; d++) {
        seq_t dd = (seq_t)d;
        seq_t d3 = dd * dd * dd;
        for (idx_t m = d; m <= n; m += d) {
            sig[m] += dd;
            if (d % 4 != 0) r4s[m] += dd;
            s3[m] += d3;
        }
    }
    seq_t *A = alloc_zero(len);
    seq_t *B = alloc_zero(len);
    seq_t *A2 = alloc_zero(len);
    seq_t *B2 = alloc_zero(len);
    A[0] = (seq_t)1;
    A2[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; k++) {
        A[k] = r4s[k] * 8;
        if (k & 1) B[k] = sig[k] * 8;
        A2[k] = s_of(s3, k) * 16;
        if ((k & 1) == 0) {
            idx_t m = k / 2;
            if (m & 1) B2[k] = (s_of(s3, k) + s_of(s3, m)) * 8;
            else B2[k] = (s_of(s3, k) - s_of(s3, m)) * 8;
        }
    }
    seq_t P = (seq_t)0, N = (seq_t)0;
    seq_t t1 = (seq_t)0, t2 = (seq_t)0, t3 = (seq_t)0, t4 = (seq_t)0;
    for (idx_t i = 0; i <= n; i++) {
        idx_t j = n - i;
        t1 += A[i] * A2[j];
        t2 += A[i] * B2[j];
        t3 += A2[i] * B[j];
        t4 += B[i] * B2[j];
    }
    P = t1 + t2 * 12;
    N = t3 * 3 + t4 * 4;
    release(sig, len);
    release(r4s, len);
    release(s3, len);
    release(A, len);
    release(B, len);
    release(A2, len);
    release(B2, len);
    return P - N;
}
