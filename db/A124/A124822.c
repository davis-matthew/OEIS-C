/*
 * A124822  a(n) = the n-th integer from among those positive integers which
 *          are coprime to n(n+1).
 * https://oeis.org/A124822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer coprime to n*(n+1)
 * Range:     u8 n <= 15, u16 n <= 396, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = the nth positive integer coprime to n*(n+1). */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1, but guard just in case */
    if (n == 0) return 0;

    /* M = n*(n+1); cast to seq_t before multiplication */ 
    seq_t M = (seq_t)n * ((seq_t)n + 1);

    idx_t target = n;        /* target count of coprimes */
    idx_t count  = 0;        /* current count */

    seq_t m = 0;
    for (;;) {
        ++m;
        if (gcd(m, M) == 1) {
            ++count;
            if (count == target) return m;
        }
    }
}
