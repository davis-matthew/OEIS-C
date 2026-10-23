/*
 * A036168  Log base 6 (n) mod 41.
 * https://oeis.org/A036168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm of n to base 6 modulo 41 (undefined
 *            for n ≡ 0 (mod 41); this implementation returns 0)
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t tbl[41];
    static bool built = false;

    if (!built) {
        /* Pre‑compute the discrete log table: tbl[r] = e such that 6^e ≡ r (mod 41) */
        uint64_t v = 1ULL;          /* 6^0 mod 41 */
        tbl[0] = 0;                 /* invalid residue; value unused */
        for (uint64_t e = 0; e < 40; ++e) {
            tbl[(size_t)v] = (seq_t)e;
            v = (v * 6ULL) % 41ULL; /* next power of 6 */
        }
        built = true;
    }

    uint64_t r = n % 41ULL;
    if (r == 0ULL) {
        /* No discrete log exists; return 0 as a safe default. */
        return 0;
    }
    return tbl[(size_t)r];
}
