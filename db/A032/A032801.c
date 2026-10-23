/*
 * A032801  Number of unordered sets a, b, c, d of distinct integers from 1..n
 *          such that a+b+c+d = 0 (mod n).
 * https://oeis.org/A032801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 263, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032801: Number of unordered sets {a,b,c,d} of distinct integers
   from 1..n such that a+b+c+d ≡ 0 (mod n).
   Formula used (derived from the OEIS entry):
       a(n) = ( C(n,4) + (n even ? C(n/2,2) : 0) - (n%4==0 ? n/2 : 0) ) / n
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;           /* no quadruples possible */
    seq_t N = (seq_t)n;
    seq_t sum = 0;

    /* C(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
    seq_t c4 = (N * (N - 1) * (N - 2) * (N - 3)) / 24;
    sum += c4;

    /* if n is even, add C(n/2,2) = (n/2)*(n/2-1)/2 */
    if ((n & 1u) == 0) {
        seq_t m = N >> 1;                /* n/2 */
        seq_t c2 = (m * (m - 1)) / 2;
        sum += c2;
    }

    /* if n divisible by 4, subtract n/2 = n/2 */
    if ((n & 3u) == 0) {
        seq_t sub = N >> 1;              /* n/2 */
        sum -= sub;
    }

    /* final division by n */
    return sum / N;
}
