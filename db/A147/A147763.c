/*
 * A147763  a(n) = smallest k such that n = d*(d-b)*(d+c), where b, c, d >= 0
 *          and k = b+c.
 * https://oeis.org/A147763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A147763: a(n) = min{c-a : n = a*b*c, 1 ≤ a ≤ b ≤ c} */
/* This implements the definition by searching over all factorizations
   into three sorted positive integers a, b, c.  For a prime n the
   minimal difference is n-1 (1·1·n).  For a perfect cube the result
   is 0 (a=b=c).  The algorithm is O(n^{2/3}) and runs well under
   two seconds for n up to about 10000. */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n).  idx_t is the index type (uint64_t).  seq_t is the
   result type (uint8_t … __int128). */
seq_t a(idx_t n)
{
    if (n == 0)          /* index below offset; return 0 */
        return (seq_t)0;
    /* For n == 1 the answer is 0 (cube 1).  The search below handles it. */

    /* Worst-case initial value: n-1 (prime case). */
    idx_t min_k = n - 1;

    /* Loop over the smallest factor a (∈ [1, ⌊cuberoot(n)⌋]). */
    for (idx_t a = 1; a <= n / a / a; ++a) {
        if (n % a != 0)
            continue;
        idx_t n1 = n / a;            /* remaining product b*c */

        /* Loop over the middle factor b (∈ [a, ⌊sqrt(n1)⌋]). */
        for (idx_t b = a; b <= n1 / b; ++b) {
            if (n1 % b != 0)
                continue;
            idx_t c = n1 / b;        /* largest factor */

            if (c < b)                /* not sorted, skip */
                continue;

            idx_t k = c - a;          /* k = y - x */
            if (k < min_k)
                min_k = k;
            if (min_k == 0)           /* cannot get better than 0 */
                return (seq_t)0;
        }
    }
    return (seq_t)min_k;
}
