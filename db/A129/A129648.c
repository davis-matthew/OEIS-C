/*
 * A129648  Largest order of a permutation of n elements with exactly 3
 *          cycles. Also the largest LCM of a 3-partition of n.
 * https://oeis.org/A129648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ lcm(a,b,c) | a+b+c = n, a,b,c ≥ 1 }
 * Range:     u8 n <= 19, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates all unordered triples (a,b,c) with sum n
 * (ensuring a ≤ b ≤ c) and keeps the maximum LCM found.  The loop
 * structure reduces the number of pairs to about n²/4, which is well
 * within the time limits for n up to ~10⁴.  All arithmetic is performed
 * using seq_t, so the code works with 8, 16, 32, 64, 128‑bit integers
 * and arbitrary‑precision integers.  It uses the provided gcd function
 * which is defined in "../utils/gcd.h" and works for all seq_t types. */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* a(1) = a(2) = 0 by definition */

    seq_t best = 0;

    for (idx_t a = 1; a <= n - 2; ++a)
    {
        /* b must satisfy a ≤ b ≤ c = n-a-b and c ≥ b, so b ≤ (n-a)/2 */
        idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b)
        {
            idx_t c = n - a - b;          /* c ≥ b by construction */

            /* Convert to seq_t for arithmetic */
            seq_t aa = (seq_t)a;
            seq_t bb = (seq_t)b;
            seq_t cc = (seq_t)c;

            /* lcm(a,b) = a / gcd(a,b) * b  (avoids overflow) */
            seq_t g_ab = gcd(aa, bb);
            seq_t lcm_ab = (aa / g_ab) * bb;

            /* lcm(a,b,c) = lcm(lcm(a,b), c) */
            seq_t g = gcd(lcm_ab, cc);
            seq_t lcm_abc = (lcm_ab / g) * cc;

            if (lcm_abc > best) best = lcm_abc;
        }
    }
    return best;
}
