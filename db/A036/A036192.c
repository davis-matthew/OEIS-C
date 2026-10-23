/*
 * A036192  Log base 5 (n) mod 157.
 * https://oeis.org/A036192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm base 5 of n modulo 157
 * Range:     u8 n <= 156, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static table mapping residue → discrete log (0 ≤ k < 156) */
    static bool init = false;
    static seq_t logtable[157];

    if (!init) {
        init = true;

        /* Compute 5^k (mod 157) for k = 0 … 155 and store the inverse map */
        uint64_t pow = 1;          /* 5^0 ≡ 1 (mod 157) */
        for (unsigned k = 0; k < 156; ++k) {
            /* `pow` is always ≤ 156, safe to cast for array index */
            logtable[(unsigned)pow] = (seq_t)k;
            pow = (pow * 5) % 157;
        }
        /* The entry logtable[0] remains 0, which will be used for multiples of 157 */
    }

    if (n == 0) return 0;          /* Undefined for n = 0, per OEIS offset */

    unsigned mod = (unsigned)(n % 157);
    /* For residues that are not powers of 5 modulo 157 (i.e. 0),
       return 0 as per the OEIS convention. */
    if (mod == 0) return 0;

    return logtable[mod];
}
