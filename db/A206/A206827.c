/*
 * A206827  Number of solutions (n,k) of s(k) == s(n) (mod n), where 1 <= k <
 *          n and s(k) = k*(k+1)*(2*k+1)/6.
 * https://oeis.org/A206827
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 ≤ k < n, with * s(k) ≡ s(n) (mod n), where
 *            s(m) = m*(m+1)*(2*m+1)/6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;

    /* s(n) modulo n */
    __int128 prod_n = (__int128)n * (n + 1) * (2 * n + 1);
    unsigned long long sn_mod = (unsigned long long)((prod_n / 6) % n);

    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        __int128 prod_k = (__int128)k * (k + 1) * (2 * k + 1);
        unsigned long long sk_mod = (unsigned long long)((prod_k / 6) % n);
        if (sk_mod == sn_mod) ++cnt;
    }
    return cnt;
}
