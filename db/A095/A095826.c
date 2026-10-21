/*
 * A095826  Numbers k such that k + largest digit of k is a triangular number.
 * https://oeis.org/A095826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 367, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095826: numbers k such that k + largest digit(k) is a triangular number.
   Computation method: simple linear (search) from k = 0, counting matches until the
   requested index n is reached.  The search is efficient enough for n ≤ 10 000. */

#include <stdint.h>

/* Return the largest decimal digit of an idx_t value. */
static inline idx_t max_digit(idx_t x)
{
    idx_t m = 0;
    do {
        idx_t d = x % 10;
        if (d > m)
            m = d;
        x /= 10;
    } while (x != 0);
    return m;
}

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(n)). */
static inline unsigned long long sqrt_uint128(unsigned __int128 n)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);  /* high > sqrt(max) */

    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return (unsigned long long)(low - 1);
}

/* Public function: a(n) returns the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t d = max_digit(k);

        /* t = k + largest_digit(k) */
        unsigned long long t = (unsigned long long)k + (unsigned long long)d;

        /* discriminant of the triangular test: 8*t + 1 */
        unsigned __int128 disc = (unsigned __int128)8 * t + (unsigned __int128)1;

        unsigned long long sq = sqrt_uint128(disc);
        if ((unsigned __int128)sq * sq == disc) {          /* t is triangular */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
