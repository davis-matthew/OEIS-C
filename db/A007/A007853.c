/*
 * A007853  Number of maximal antichains in rooted plane trees on n nodes.
 * https://oeis.org/A007853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1))*C(2*n,n) + Sum_{k=0..n-1}
 *            ((k+2)/(n+1))*C(2*n-k-1,n-k-1)*T(k), a(n) here meaning OEIS
 *            a(n+1), where T(k) = Sum_{i=0..floor(k/2)}
 *            C(2*i,i)*C(k+i,3*i)/(i+1) satisfies (k+2)*T(k) = 2*(2k+1)*T(k-1)
 *            - 2*(k-1)*T(k-2) - (k-4)*T(k-4), T(0..3) = 1,1,2,5; and B(n,k) =
 *            ((k+2)/(n+1))*C(2n-k-...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, gcd_u64, mul_div, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/gcd_u64.h"
#include "../utils/mul_div.h"

seq_t a(idx_t idx) {
    if (idx <= 1) return 1;
    idx_t n = idx - 1;
    idx_t len = n < 4 ? 4 : n;
    seq_t *T = alloc_zero(len);
    T[0] = 1; T[1] = 1; T[2] = 2; T[3] = 5;
    for (idx_t k = 4; k < len; k++) {
        seq_t num = (seq_t)(2 * (2 * k + 1)) * T[k - 1];
        num -= (seq_t)(2 * (k - 1)) * T[k - 2];
        num -= (seq_t)(k - 4) * T[k - 4];
        T[k] = num / (seq_t)(k + 2);
    }
    seq_t sum = 0;
    seq_t B = 1;
    for (idx_t k = n; k-- > 0;) {
        sum += T[k] * B;
        if (k > 0) {
            uint64_t p = (uint64_t)(k + 1) * (uint64_t)(2 * n - k);
            uint64_t q = (uint64_t)(k + 2) * (uint64_t)(n - k);
            B = mul_div(B, p, q);
        }
    }
    sum += B;
    release(T, len);
    return sum;
}
