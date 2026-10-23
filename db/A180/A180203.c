/*
 * A180203  Differences between prime powers of primes, offsetting the prime
 *          and the power by only one. (For purposes of this sequence, 0 and 1
 *          are treated as primes; see Formula.)
 * https://oeis.org/A180203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = EP(n+1)^EP(n) - EP(n)^EP(n-1), where EP(1)=0, EP(2)=1,
 *            EP(k)=Prime(k-2) for k>2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

static inline seq_t EP(idx_t k) {
    if (k == 1) return (seq_t)0;
    if (k == 2) return (seq_t)1;
    return (seq_t)get_prime(k - 2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined, but harmless */
    if (n == 1) return (seq_t)1;     /* special case */

    seq_t ep_n_minus1 = EP(n - 1);
    seq_t ep_n        = EP(n);
    seq_t ep_n_plus1  = EP(n + 1);

    seq_t term1 = big_pow(ep_n_plus1, ep_n);
    seq_t term2 = big_pow(ep_n, ep_n_minus1);
    return term1 - term2;
}
