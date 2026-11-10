/*
 * A064590  Ordered values of A064494.
 * https://oeis.org/A064590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over v = 1, 2, 3, ...; a(n) is the n-th v that is a value
 *            of A064494. Membership is tested by following entry v forward
 *            through T[2], T[3], ...: at step k with position p, p == 2k (or
 *            p < k) means v is fixed (member); k < p < 2k means v escapes
 *            forever (non-member); otherwise if k | p, p m...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A064590_is_shotgun_value(uint64_t v)
{
    uint64_t pos = v;
    for (uint64_t k = 2; ; ++k) {
        if (pos < k) return true;          /* already below the swap range: fixed */
        if (pos == 2 * k) return true;     /* moves to position k, then fixed      */
        if (pos < 2 * k) return false;     /* k <= pos < 2k: doubles forever       */
        if (pos % k == 0) {
            uint64_t t = pos / k;
            if (t & 1u) pos += k;
            else pos -= k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    uint64_t v = 0;
    while (count < n) {
        ++v;
        if (A064590_is_shotgun_value(v)) ++count;
    }
    return (seq_t)v;
}
