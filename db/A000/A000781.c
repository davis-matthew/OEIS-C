/*
 * A000781  a(n) = 3*Catalan(n) - Catalan(n-1) - 1.
 * https://oeis.org/A000781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 3*Catalan(n) - Catalan(n-1) - 1, with Catalan(k) =
 *            Catalan(k-1)*(4k-2)/(k+1), Catalan(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   a781_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a781_gcd.h"

seq_t a(idx_t n) {
    static idx_t sk = 0;      /* index of cached Catalan value */
    static seq_t sc = 1;      /* Catalan(sk) */
    static seq_t sp = 0;      /* Catalan(sk-1) */

    if (n < sk) {
        sk = 0;
        sc = 1;
        sp = 0;
    }
    while (sk < n) {
        idx_t k = sk + 1;
        uint64_t m = (uint64_t)k + 1;
        uint64_t r = (uint64_t)(sc % (seq_t)m);
        uint64_t g = a781_gcd(m, r);
        seq_t q = sc / (seq_t)g;
        uint64_t d = m / g;
        uint64_t f = (4 * (uint64_t)k - 2) / d;
        seq_t nc = q * (seq_t)f;
        sp = sc;
        sc = nc;
        sk = k;
    }
    return (seq_t)3 * sc - sp - 1;
}
