/*
 * A155874  Smallest positive composite number such that a(n)+n is also
 *          composite.
 * https://oeis.org/A155874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest positive composite number c such that c + n is composite.
 * For n ≥ 0, c ∈ {4, 6, 8}.  c = 4 if n + 4 is composite,
 * otherwise c = 6 if n + 6 is composite,
 * otherwise c = 8.  This implements the rule given in OEIS A155874. */

#include <stdbool.h>

static inline bool is_composite(idx_t x)
{
    /* 1 and 2,3 are not composite.  All other numbers are composite
     * iff they are divisible by some integer d, 2 ≤ d ≤ sqrt(x).  */
    if (x <= 3) return false;
    if ((x & 1ULL) == 0) return true; /* even numbers > 2 are composite */

    for (idx_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return true;
    }
    return false; /* prime */
}

seq_t a(idx_t n)
{
    if (is_composite(n + 4)) return (seq_t)4;
    if (is_composite(n + 6)) return (seq_t)6;
    return (seq_t)8;
}
