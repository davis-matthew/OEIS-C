/*
 * A107628  Number of integral quadratic forms ax^2 + bxy + cy^2 whose
 *          discriminant b^2-4ac is -n, 0 <= b <= a <= c and gcd(a,b,c) = 1.
 * https://oeis.org/A107628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of primitive binary quadratic forms ax^2 + bxy + cy^2
   with discriminant D = b^2 - 4ac = -n, satisfying 0 ≤ b ≤ a ≤ c.
   This is OEIS sequence A107628. */

#include <stdint.h>

static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset starts at 1 */

    seq_t count = 0;

    /* compute maximum possible a: 3*a^2 ≤ n   (since b ≤ a ≤ c and D = -n) */
    idx_t a_max = 0;
    for (idx_t a = 1; ; ++a) {
        unsigned __int128 lhs = (unsigned __int128)3 * a * a;
        if (lhs > n) break;
        a_max = a;
    }

    for (idx_t a = 1; a <= a_max; ++a) {
        unsigned __int128 denom = (unsigned __int128)4 * a;
        for (idx_t b = 0; b <= a; ++b) {
            unsigned __int128 sum = (unsigned __int128)b * b + n; /* b² + n */
            if (sum % denom != 0) continue;                      /* c must be integer */

            unsigned __int128 c128 = sum / denom;
            uint64_t c = (uint64_t)c128;
            if (c < a) continue;                                 /* need a ≤ c */

            /* gcd(a,b,c) must be 1 */
            uint64_t g = gcd64((uint64_t)a, (uint64_t)b);
            g = gcd64(g, c);
            if (g == 1) count += 1;
        }
    }
    return count;
}
