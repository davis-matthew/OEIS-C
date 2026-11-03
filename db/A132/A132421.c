/*
 * A132421  a(n) = LCM of the integers b(k), over all k where 1 <= k <= n,
 *          where b(k) = the k-th integer from among those positive integers
 *          which are coprime to (n+1-k).
 * https://oeis.org/A132421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of the k-th integer coprime to (n+1-k), for 1 ≤ k ≤ n
 *            Computation: for each k, enumerate integers until k coprime to
 *            m=n+1-k are found, then lcm of all such integers
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build system.  No external libraries
   are used aside from the standard C headers. */
#include <stdint.h>
#include "../utils/gcd.h"

/* Return the k-th positive integer that is coprime to m (m ≥ 1). */
static seq_t kth_coprime(idx_t k, idx_t m)
{
    seq_t count = 0;
    seq_t num   = 1;            /* candidate integer */
    seq_t sm    = (seq_t)m;     /* cast once for speed */

    while (1)
    {
        if (gcd(num, sm) == 1)
        {
            ++count;
            if (count == k)
                return num;
        }
        ++num;
    }
}

/* Least common multiple of two seq_t values. */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    if (g == 0)
        return 0;
    return (a / g) * b;
}

/* Greatest common divisor for seq_t using the Euclidean algorithm. */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1; graceful fallback */

    seq_t lcm_val = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = n + 1 - k;
        seq_t bk = kth_coprime(k, m);
        lcm_val = lcm_seq(lcm_val, bk);
    }

    return lcm_val;
}
