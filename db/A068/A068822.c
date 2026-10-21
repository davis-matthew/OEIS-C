/*
 * A068822  a(n) = gcd(n,c(n)), where c(n) is the 10's complement of n.
 * https://oeis.org/A068822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = gcd(n, 10^digits(n))
 * Range:     u8 n <= 499, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                --- using the fact that gcd(n,10^d-n)=gcd(n,10^d) */

/* The sequence can be computed for any n ≥ 1 using only
   operations on the index type (idx_t) and the sequence
   type (seq_t).  The value a(n) is a divisor of n, therefore
   its size never exceeds the size of the index.  The
   implementation only requires bit‑wise manipulation and
   a few loops, making the function run in O(log n) time. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* not needed for OEIS offset 1 */

    /* Copy n to unsigned 64‑bit variable for processing. */
    uint64_t num = n;

    /* Count decimal digits of n. */
    int digits = 0;
    uint64_t tmp = num;
    do {
        digits++;
        tmp /= 10;
    } while (tmp != 0);

    /* Count exponent of 2 in n. */
    int v2 = 0;
    uint64_t t = num;
    while ((t & 1ULL) == 0) {
        v2++;
        t >>= 1;
    }

    /* Count exponent of 5 in n. */
    int v5 = 0;
    t = num;
    while (t % 5ULL == 0 && t != 0) {
        v5++;
        t /= 5;
    }

    /* gcd(n,10^digits) = 2^min(v2,digits) * 5^min(v5,digits). */
    int exp2 = v2 < digits ? v2 : digits;
    int exp5 = v5 < digits ? v5 : digits;

    seq_t result = 1;
    for (int i = 0; i < exp2; ++i)
        result = result * (seq_t)2;
    for (int i = 0; i < exp5; ++i)
        result = result * (seq_t)5;

    return result;
}
