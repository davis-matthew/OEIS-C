/*
 * A125748  a(n) is the smallest positive integer such that Sum_{1<=k<=a(n),
 *          gcd(k,n)=1} k is >= n.
 * https://oeis.org/A125748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that Σ_{1≤k≤m,
 *            gcd(k,n)=1} k ≥ n
 * Range:     u8 n <= 575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any n ≥ 1.  Uses only integer
   arithmetic and the provided gcd routine. */
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) {  /* Undefined for n = 0, but guard anyway */
        return (seq_t)0;
    }
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        if (gcd((seq_t)k, n_seq) == (seq_t)1) {
            sum += (seq_t)k;
            if (sum >= n_seq) {
                return (seq_t)k;
            }
        }
    }
}
