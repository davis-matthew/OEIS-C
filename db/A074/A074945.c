/*
 * A074945  Number of k with 1<=k<=n such that gcd(n,k) = floor(n/k).
 * https://oeis.org/A074945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 520, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ugcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the number of integers k with 1 ≤ k ≤ n such that
   gcd(n,k) = floor(n/k).  Brute‑force search over k.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ugcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset, safe guard */

    seq_t count = 0;
    unsigned long long un = (unsigned long long)n;

    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long q = un / k;                /* floor(n/k)          */
        unsigned long long g = ugcd(un, (unsigned long long)k);
        if (g == q) ++count;
    }
    return count;
}
