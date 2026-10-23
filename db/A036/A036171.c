/*
 * A036171  Log base 2 (n) mod 53.
 * https://oeis.org/A036171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete log base 2 of n modulo 53, that is, the unique k
 *            ∈ {0,…,52} such that 2^k ≡ n (mod 53); if n ≡ 0 (mod 53) the
 *            function returns 0
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t table[53];
    static bool inited = false;

    if (!inited) {
        const idx_t mod = 53ULL;
        idx_t val = 1;                 /* 2^0 mod 53 */
        for (idx_t e = 0; e < 52; ++e) {
            table[(size_t)val] = (seq_t)e;
            val = (val * 2) % mod;     /* next power of 2 modulo 53 */
        }
        inited = true;
    }

    const idx_t r = n % 53ULL;
    return table[(size_t)r];
}
