/*
 * A161396  a(n) = the smallest positive integer that contains the same number
 *          of 1's as n when a(n) and n are written in binary, is not coprime
 *          to n, and is not a divisor of n.
 * https://oeis.org/A161396
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A161396: a(n) = smallest integer m > 0 s.t.
   popcount(m) == popcount(n),
   gcd(m, n) > 1,
   and n % m != 0.
   Computed by brute‑force enumerating numbers with the same popcount as n. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are irrelevant */

    /* popcount of n */
    unsigned int k = __builtin_popcountll((unsigned long long)n);
    if (k == 0) return 0;         /* safety check – should never happen for n>=2 */

    seq_t sn = (seq_t)n;          /* convert n to seq_t once */
    seq_t one = (seq_t)1;

    /* first candidate with k ones: (1 << k) – 1 */
    seq_t m = (one << k) - one;

    for (;;)
    {
        /* check the three conditions */
        if (gcd(m, sn) > one && (sn % m) != 0)
            return m;

        /* generate next integer with the same popcount */
        seq_t u = m & (-m);                    /* lowest set bit */
        seq_t v = m + u;                       /* m + lowest set bit */
        m = v + (((v ^ m) / u) >> 2);          /* Gosper's hack for next mask */

        /* if m becomes zero it means we exhausted all combinations;
           it should never happen for the required values. */
        if (m == 0) break;
    }

    /* should not reach here for n >= 2 */
    return 0;
}
