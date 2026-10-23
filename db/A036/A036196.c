/*
 * A036196  Log base 2 (n) mod 179.
 * https://oeis.org/A036196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm of (n mod 179) to base 2 modulo 179.
 *            That is, a(n) is the unique e (0 ≤ e ≤ 178) such that 2^e ≡ n
 *            (mod 179). For n≡0 (mod 179) the function returns 0. Powers of
 *            two give the expected results: a(2^k)=k
 * Range:     u8 n <= 178, u16 n <= 178, u32 n <= 178, u64 n <= 178,
 *            u128 n <= 178, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t tbl[179];

    if (!initialized) {
        seq_t val = 1;                 /* 2^0 mod 179 */
        tbl[1] = 0;                    /* exponent for residue 1 */
        for (int e = 1; e < 178; ++e) {
            val = (val * 2) % 179;
            tbl[(size_t)val] = (seq_t)e;
        }
        tbl[0] = 0;                    /* convention for residue 0 */
        initialized = 1;
    }

    idx_t r = n % 179;                 /* reduce modulo 179 */
    if (r == 0) {
        return 0;
    }
    return tbl[(size_t)r];
}
