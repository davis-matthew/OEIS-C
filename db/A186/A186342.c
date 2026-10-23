/*
 * A186342  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the pentagonal numbers and
 *          the octagonal numbers. Complement of A186343.
 * https://oeis.org/A186342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + |{k | O(k) < P(n)}|, where P(n) = n(3n−1)/2
 *            (pentagonal number) O(k) = k(3k−2) (octagonal number). Compute k
 *            = floor((sqrt(4+12P)+2)/6); If P(n) equals O(k) subtract one to
 *            exclude the equal octagonal
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* pentagonal number P(n) = n(3n−1)/2 */
    seq_t P = nn * ((seq_t)3 * nn - (seq_t)1) / (seq_t)2;

    /* compute k = floor((sqrt(4+12P)+2)/6) */
    seq_t D = (seq_t)12 * P + (seq_t)4;           /* 4 + 12P */
    seq_t sqrtD = floor_sqrt(D);                 /* floor sqrt of D   */
    seq_t k = (sqrtD + (seq_t)2) / (seq_t)6;      /* floor division    */

    /* adjust if P(n) is itself an octagonal number */
    if (k > (seq_t)0) {
        seq_t O_k = k * ((seq_t)3 * k - (seq_t)2); /* O(k) = k(3k−2) */
        if (O_k == P) {
            k -= (seq_t)1;
        }
    }

    return nn + k;
}
