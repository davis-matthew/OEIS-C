/*
 * A024361  Number of primitive Pythagorean triangles with leg n.
 * https://oeis.org/A024361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(ω(n)-1) if n ≠ 2 (mod 4) and n > 1, else 0 where ω(n)
 *            is the number of distinct prime factors of n
 * Range:     u8 n <= 9899, u16 n <= 9899, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n <= 1 → 0 */
    if (n <= 1) return 0;

    /* n ≡ 2 (mod 4) → 0 */
    if ((n & 3) == 2) return 0;

    idx_t temp = n;
    idx_t factor_count = 0;

    /* handle factor 2 */
    if ((temp & 1) == 0) {
        factor_count++;
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            factor_count++;
            while (temp % p == 0) temp /= p;
        }
    }

    /* remaining prime > 1 */
    if (temp > 1) factor_count++;

    /* factor_count >= 1 for n > 1, so safe */
    return pow2(factor_count - 1);
}
