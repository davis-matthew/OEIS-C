/*
 * A074591  If n is a prime power then 0 else n.
 * https://oeis.org/A074591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 9527, u32 n <= 9527, u64 n <= 9527,
 *            u128 n <= 9527, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 if n is a prime power, otherwise a(n) = n.  */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t x = (uint64_t)n;
    uint64_t p = 0;

    /* Find the smallest prime factor. */
    if ((x & 1ULL) == 0) {                /* n even */
        p = 2;
    } else {
        for (uint64_t d = 3; d <= x / d; d += 2) {
            if (x % d == 0) { p = d; break; }
        }
    }

    if (p == 0) return (seq_t)0;          /* n is prime, thus a prime power */

    /* Strip all occurrences of the found prime factor. */
    while (x % p == 0) x /= p;

    return (x == 1) ? (seq_t)0 : (seq_t)n; /* 0 if n is a prime power */
}
