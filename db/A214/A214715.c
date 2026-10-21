/*
 * A214715  Least m>0 such that prime(n+1)-m and n-m are relatively prime.
 * https://oeis.org/A214715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that gcd(prime(n+1)-m, n-m)=1
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    // Obtain the (n+1)-th prime
    uint32_t p_u32 = get_prime(n + 1);
    seq_t p = (seq_t)p_u32;

    // Search for the smallest m>0 satisfying the coprimality condition
    for (seq_t m = 1; ; ++m) {
        seq_t diff_p = (p > m) ? (p - m) : (m - p);     // |p - m|
        seq_t diff_n = ((seq_t)n > m) ? ((seq_t)n - m) : (m - (seq_t)n); // |n - m|
        if (gcd(diff_p, diff_n) == 1)
            return m;
    }
}
