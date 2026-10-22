/*
 * A002463  Coefficients of Legendre polynomials.
 * https://oeis.org/A002463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n-2,n-1)*binomial(2n,n) / 2^A000120(2n-1)
 *            (A000120 = binary weight). Computed as
 *            odd(C(2n-2,n-1))*odd(C(2n,n))*2^(A000120(n)-1), where
 *            odd(C(2k,k)) = odd(C(2k-2,k-1))*(2k-1)/odd(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   a002463_pop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002463_pop.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t o = 1, o1 = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        idx_t ok = k;
        while ((ok & 1) == 0) ok >>= 1;
        if (k == n) o1 = o;
        o = o * (seq_t)(2 * k - 1);
        o = o / (seq_t)ok;
    }
    seq_t r = o1 * o;
    int e = a002463_pop(n) - 1;
    while (e-- > 0) r = r * 2;
    return r;
}
