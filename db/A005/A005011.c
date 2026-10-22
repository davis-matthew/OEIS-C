/*
 * A005011  Shifts one place left under 5th-order binomial transform.
 * https://oeis.org/A005011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, eigen
 * Formula:   a(n) = Sum_{m=0..n} 5^(n-m)*Stirling2(n, m), computed with
 *            T(n,m) = 5^(n-m)*Stirling2(n,m), T(n,m) = 5*m*T(n-1,m) +
 *            T(n-1,m-1), T(0,0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *T = alloc_zero(len);
    seq_t result = 0;
    idx_t i, m;

    T[0] = 1;
    for (i = 1; i <= n; i++) {
        for (m = i; m >= 1; m--) {
            T[m] = (seq_t)5 * (seq_t)m * T[m] + T[m - 1];
        }
        T[0] = 0;
    }

    for (m = 0; m <= n; m++) {
        result += T[m];
    }

    release(T, len);
    return result;
}
