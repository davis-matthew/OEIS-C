/*
 * A103528  a(n) = Sum_{k = 1..n-1 such that n == k (mod 2^k)} 2^(k-1).
 * https://oeis.org/A103528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..n-1, n ≡ k (mod 2^k)} 2^(k-1)
 * Range:     u8 n <= 520, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n <= 1)
        return 0;          /* first term a(1) = 0 */

    /* iterate over possible k. For k > 63 shifting 1ULL << k is undefined,
       and for k such that 2^k > n the congruence cannot hold because
       n < 2^k and k > n. */
    for (idx_t k = 1; k < 64; ++k) {
        uint64_t mod = 1ULL << k;
        if (mod > n)          /* 2^k > n: no more valid k */
            break;
        if ((n % mod) == k) {
            sum += ((seq_t)1 << (k - 1));  /* add 2^(k-1) */
        }
    }
    return sum;
}
