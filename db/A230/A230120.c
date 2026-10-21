/*
 * A230120  a(n) is the number of evil integers (A001969) not exceeding n and
 *          prime to n.
 * https://oeis.org/A230120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = count of evil integers ≤ n that are coprime to n (OEIS
 *            A230120)
 * Range:     u8 n <= 520, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Counts the number of 1-bits in the binary representation of x.
   Returns true if this count is even (evil), false otherwise.
   Uses compiler builtins when available for speed. */
static inline bool is_evil(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    unsigned cnt = __builtin_popcountll(x);
#else
    unsigned cnt = 0;
    while (x) {
        cnt += (unsigned)(x & 1);
        x >>= 1;
    }
#endif
    return (cnt & 1) == 0;
}

/* Euclidean algorithm for greatest common divisor, using idx_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t count = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd_idx(i, n) == 1 && is_evil(i))
            count += (seq_t)1;
    }
    return count;
}
