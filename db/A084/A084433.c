/*
 * A084433  Numbers whose cyclic permutations have GCD > 1.
 * https://oeis.org/A084433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 4471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084433: Numbers whose cyclic decimal permutations have GCD > 1
 *
 * A number x satisfies the condition if the GCD of all cyclic
 * permutations of its decimal digits is > 1.  The function returns the
 * n‑th smallest such integer for n ≥ 1 (a(1) = 2).  The implementation
 * uses a straightforward search, evaluating each integer in turn and
 * testing the required property with the Euclidean algorithm for
 * gcd.  This works for all supported seq_t types: 8, 16, 32, 64 or
 * 128‑bit unsigned integers or arbitrary‑precision integer classes
 * provided by the build.
 */

#include <stdbool.h>
#include "../utils/seq_digit_len.h"

/* Euclidean algorithm for seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* True if all cyclic rotations of x share a common divisor > 1 */
static inline bool has_common_factor(seq_t x)
{
    unsigned int d = seq_digit_len(x);
    seq_t pow10 = (seq_t)1;
    for (unsigned int i = 1; i < d; ++i)
        pow10 *= (seq_t)10;

    seq_t g = x;
    seq_t cur = x;
    for (unsigned int i = 1; i < d; ++i) {
        cur = (cur % pow10) * (seq_t)10 + cur / pow10;
        g = seq_gcd(g, cur);
        if (g == (seq_t)1)
            return false;          /* early exit */
    }
    return g > (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;           /* undefined for n = 0 */

    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)2;      /* start from the smallest integer > 1 */

    while (true) {
        if (has_common_factor(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
