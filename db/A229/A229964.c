/*
 * A229964  Number of pairs of integers q1, q2 with 1 < q1 < q2 < n such that
 *          if we randomly pick an integer in {1, ..., n}, the event of being
 *          divisible by q1 is independent of being divisible by q2.
 * https://oeis.org/A229964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_gcd_seq.h"
/* OEIS A229964: a(n) = # { (q1,q2) | 1<q1<q2<n and
   n * floor(n/lcm(q1,q2)) = floor(n/q1) * floor(n/q2) }.
   This implementation evaluates the definition directly by
   exhaustive search. */

seq_t a(idx_t n)
{
    if (n <= 3)   /* no pairs possible for n=1,2,3 */
        return (seq_t)0;

    seq_t count = (seq_t)0;
    seq_t nn = (seq_t)n;

    /* Iterate over all unordered pairs 1<q1<q2<n */
    for (idx_t q1 = 2; q1 < n; ++q1) {
        seq_t q1s = (seq_t)q1;
        seq_t a_val = nn / q1s;   /* floor(n/q1) */

        for (idx_t q2 = q1 + 1; q2 < n; ++q2) {
            seq_t q2s = (seq_t)q2;
            seq_t b_val = nn / q2s;   /* floor(n/q2) */

            seq_t g = _gcd_seq(q1s, q2s);
            seq_t l = (q1s / g) * q2s;    /* lcm(q1,q2) */

            seq_t left  = nn * (nn / l);  /* n * floor(n/lcm) */
            seq_t right = a_val * b_val;  /* floor(n/q1) * floor(n/q2) */

            if (left == right)
                ++count;
        }
    }

    return count;
}
