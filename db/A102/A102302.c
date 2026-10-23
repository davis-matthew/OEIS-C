/*
 * A102302  Largest number < n/2 coprime to n.
 * https://oeis.org/A102302
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max{k < n/2 : gcd(k,n)=1}
 * Range:     u8 n <= 512, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute gcd of two idx_t values (iterative Euclid) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* a(idx_t n) returns the largest integer k < n/2 such that gcd(k,n)=1.
   For n < 7 the return value is unspecified (0 is returned). */
seq_t a(idx_t n)
{
    if (n < 7) {
        return 0;          /* sequence offset is 7 */
    }

    /* largest integer strictly less than n/2 */
    idx_t start = (n - 1) / 2;

    /* search downwards for a coprime k */
    for (idx_t k = start;; --k) {
        if (gcd_idx(n, k) == 1) {
            return (seq_t)k;
        }
        if (k == 1) {
            break;          /* no smaller k exists */
        }
    }
    return 0;              /* should never be reached for n >= 7 */
}
