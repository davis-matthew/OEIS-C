/*
 * A051540  Least common multiple of {2, 5, 8, 11, 14, ..., 3n+2} (A016789).
 * https://oeis.org/A051540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051540 – Least common multiple of {2,5,8,11,…,3n+2}
   Formula: a(n) = lcm{ 3*k + 2 | 0 ≤ k ≤ n } */

#include <stdint.h>

/* Helper: Euclidean algorithm for seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;

    /* Iterate over 3*k+2 for all k from 0 to n */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t = (seq_t)3 * (seq_t)k + (seq_t)2;
        seq_t g = gcd_seq_t(result, t);
        /* lcm(a,b) = a / g * b */
        result = (result / g) * t;
    }

    return result;
}
