/*
 * A005259  Apery (Apéry) numbers: Sum_{k=0..n}
 *          (binomial(n,k)*binomial(n+k,k))^2.
 * https://oeis.org/A005259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n,k)*binomial(n+k,k))^2, with t(k)
 *            = C(n,k)*C(n+k,k) updated by t(k+1) =
 *            t(k)*(n-k)/(k+1)*(n+k+1)/(k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t t = 1;          /* C(n,0)*C(n,0) */
    seq_t result = 1;     /* k = 0 term squared */
    for (idx_t k = 0; k < n; k++) {
        /* C(n,k)*(n-k)/(k+1) = C(n,k+1): exact division */
        t = t * (n - k) / (k + 1);
        /* C(n+k,k)*(n+k+1)/(k+1) = C(n+k+1,k+1): exact division */
        t = t * (n + k + 1) / (k + 1);
        result += t * t;
    }
    return result;
}
