/*
 * A169883  Maximum number of rational points on a smooth absolutely
 *          irreducible projective curve of genus 1 over the field F_7^n.
 * https://oeis.org/A169883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n + 1 + ⌊2·7^(n/2)⌋ if (n is even or n = 1 or
 *            ⌊2·7^(n/2)⌋ mod 7 ≠ 0); * a(n) = 7^n + ⌊2·7^(n/2)⌋ otherwise.
 *            (Deuring–Waterhouse)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 1. Compute p = 7^n. */
    seq_t p = big_pow((seq_t)7, (seq_t)n);

    /* 2. Compute m = ⌊√p⌋ = ⌊7^(n/2)⌋. */
    seq_t m = floor_sqrt(p);

    /* 3. Determine whether ⌊2·7^(n/2)⌋ is 2·m or 2·m + 1.
       For odd n it is 2·m + 1 iff (m + 0.5)^2 ≤ p, i.e.
       4·m² + 4·m + 1 ≤ 4·p.  For even n the inequality fails,
       giving 2·m as the exact value. */
    seq_t m2   = m * m;                     /* m² */
    seq_t left = m2 * 4 + m * 4 + (seq_t)1; /* 4·m² + 4·m + 1 */
    seq_t fourp = p * 4;                    /* 4·p */
    seq_t t;                               /* ⌊2·7^(n/2)⌋ */
    if (left <= fourp)
        t = m * 2 + (seq_t)1;              /* 2·m + 1 */
    else
        t = m * 2;                         /* 2·m */

    /* 4. Decide whether to add the extra +1. */
    bool add_flag = (n == 1) || ((n & 1) == 0); /* n even or n = 1 */
    if (!add_flag && (t % (seq_t)7) != (seq_t)0)
        add_flag = true;                    /* odd n > 1 and t not divisible by 7 */

    /* 5. Compute the final value. */
    seq_t ans = p + t;
    if (add_flag)
        ans += (seq_t)1;

    return ans;
}
