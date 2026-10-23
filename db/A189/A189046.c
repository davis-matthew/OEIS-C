/*
 * A189046  a(n) = lcm(n,n+1,n+2,n+3,n+4,n+5)/60.
 * https://oeis.org/A189046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,n+1,n+2,n+3,n+4,n+5)/60
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 119, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0) return (seq_t)0;
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    seq_t l = (seq_t)n;
    for (idx_t i = 1; i <= 5; ++i) {
        l = lcm_seq(l, (seq_t)(n + i));
    }
    /* lcm of six consecutive integers is always divisible by 60 for n≥1 */
    return l / (seq_t)60;
}
