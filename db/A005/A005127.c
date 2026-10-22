/*
 * A005127  Number of k for which n does not divide Stirling cycle numbers [
 *          {n \atop k} ].
 * https://oeis.org/A005127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 0<=k<=n, n does not divide c(n,k)}, where c(n,k)
 *            are the unsigned Stirling numbers of the first kind, computed
 *            mod n by c(i,k) = c(i-1,k-1) + (i-1)*c(i-1,k), c(0,0) = 1; a(0)
 *            = 1
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    uint64_t *c = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    c[0] = 1 % m;
    for (uint64_t i = 1; i <= m; i++) {
        uint64_t mult = (i - 1) % m;
        for (uint64_t k = i; k >= 1; k--) {
            c[k] = (c[k - 1] + mult * c[k]) % m;
        }
        c[0] = (mult * c[0]) % m;
    }
    uint64_t count = 0;
    for (uint64_t k = 0; k <= m; k++)
        if (c[k] != 0) count++;
    free(c);
    return (seq_t)count;
}
