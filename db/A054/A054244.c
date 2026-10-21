/*
 * A054244  Number of partitions of n into distinct positive parts <= n, with
 *          parts combined by IOR (inclusive or).
 * https://oeis.org/A054244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = A003465(A000120(n)) = Σ_{t=0}^{k} (-1)^t * C(k,t) *
 *            2^{2^{k-t}-1}, where k = popcount(n)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=1, a(3)=5, a(4)=1, a(5)=5, ... */

#include <stdint.h>
#include <limits.h>

/* Helper: compute binomial coefficient C(n,k) for n <= 64.
   The result fits in 64 bits. */
static uint64_t binom_uint64(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* n >= 1 */
    /* compute popcount of n (index may be 64-bit unsigned). */
    uint64_t nn = (uint64_t)n;
    uint64_t k = __builtin_popcountll(nn);  /* number of set bits */

    seq_t sum = (seq_t)0;
    for (uint64_t t = 0; t <= k; ++t) {
        uint64_t comb = binom_uint64(k, t);
        /* exponent = 2^{k-t} - 1.  For n <= 10000, k-t <= 14, so fits 32 bits. */
        uint64_t pow2 = (uint64_t)1 << (k - t);   /* 2^{k-t} */
        uint64_t exp = pow2 - 1;                 /* 2^{k-t} - 1 */
        /* term = 2^{exp}  (sequence type supports shift by 64-bit). */
        seq_t term = (seq_t)1 << exp;
        seq_t contrib = (seq_t)comb * term;
        if ((t & 1) == 0)
            sum += contrib;
        else
            sum -= contrib;
    }
    return sum;
}
