/*
 * A033726  Product theta3(q^d); d | 14.
 * https://oeis.org/A033726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j,k,l) ∈ ℤ⁴ | i² + 2 j² + 7 k² + 14 l² = n} Each
 *            solution is counted with multiplicity 2ᶻ, where z is the number
 *            of non‑zero absolute values among i,j,k,l. (i.e. each non‑zero
 *            coordinate contributes two signs.)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* iterate over l (coefficient 14) */
    for (idx_t l = 0; ; ++l) {
        idx_t suml = 14ULL * l * l;
        if (suml > n) break;

        /* iterate over k (coefficient 7) */
        for (idx_t k = 0; ; ++k) {
            idx_t sumk = suml + 7ULL * k * k;
            if (sumk > n) break;

            /* iterate over j (coefficient 2) */
            for (idx_t j = 0; ; ++j) {
                idx_t sumj = sumk + 2ULL * j * j;
                if (sumj > n) break;

                /* remaining part that must be filled by i */
                for (idx_t i = 0; ; ++i) {
                    idx_t sum = sumj + i * i;
                    if (sum > n) break;

                    if (sum == n) {
                        /* compute multiplicity */
                        int cnt = (i > 0) + (j > 0) + (k > 0) + (l > 0);
                        result += ((seq_t)1 << cnt);
                    }
                }
            }
        }
    }

    return result;
}
