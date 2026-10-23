/*
 * A151790  Partial sums of A151789.
 * https://oeis.org/A151790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 5 * Σ_{m=1}^{n-1} 4^{popcount(m)-1} where popcount(m)
 *            is the number of 1-bits in the binary representation of m. This
 *            is the partial sum of A151789, defined by b(1)=1, and for k≥2,
 *            b(k)=5·4^{popcount(k-1)-1}
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, but harmless */
    /* Pre-compute powers of 4 up to 64 bits (enough for idx_t) */
    static seq_t pow4[65];
    static int initialized = 0;
    if (!initialized) {
        pow4[0] = (seq_t)1;
        for (int i = 1; i < 65; ++i)
            pow4[i] = pow4[i-1] * (seq_t)4;
        initialized = 1;
    }

    seq_t total = (seq_t)1;              /* a(1) = 1 */
    for (idx_t m = 1; m < n; ++m) {
        unsigned int pc = __builtin_popcountll((unsigned long long)m);
        /* pc ≥ 1 for m ≥ 1 */
        seq_t term = (seq_t)5 * pow4[pc-1];
        total += term;
    }
    return total;
}
