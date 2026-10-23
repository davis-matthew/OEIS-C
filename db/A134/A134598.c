/*
 * A134598  Duplicate of A080773.
 * https://oeis.org/A134598
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = sum_{prime factors p of n, counted with multiplicity}
 *            popcount(p)
 * Range:     i8 n <= 106, i16 n <= 106, i32 n <= 106, i64 n <= 106,
 *            i128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where popcount(p) is the number of 1‑bits in the binary representation of p.   */

#include <stdint.h>

static inline int popcount_u64(uint64_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n<2, return 0 */

    idx_t temp = n;
    seq_t sum = 0;

    /* factor out 2 */
    while ((temp & 1ULL) == 0) {
        sum += 1;                 /* popcount(2) == 1 */
        temp >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        while (temp % p == 0) {
            sum += (seq_t)popcount_u64(p);
            temp /= p;
        }
    }

    /* any remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        sum += (seq_t)popcount_u64(temp);
    }

    return sum;
}
