/*
 * A084385  a(1) = 1; a(n+1) is the smallest number not occurring earlier and
 *          coprime to Sum_{j=1..n} a(j).
 * https://oeis.org/A084385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1 ; a(n+1) = smallest integer not seen before that is
 *            coprime to S(n)=∑_{i=1..n}a(i). For n > 6: a(n) = n-2 if n mod 4
 *            = 0 a(n) = n-1 if n mod 4 = 1 a(n) = n+1 if n mod 4 = 2 a(n) =
 *            n+2 if n mod 4 = 3 The values for n ≤ 6 are {1,2,4,3,7,5}
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 6) {
        static const seq_t init[6] = {1, 2, 4, 3, 7, 5};
        return init[n - 1];
    }

    seq_t t = (seq_t)n;
    switch (n & 3ULL) {          /* n mod 4 */
        case 0:  return t - (seq_t)2;
        case 1:  return t - (seq_t)1;
        case 2:  return t + (seq_t)1;
        case 3:  return t + (seq_t)2;
    }
    return 0; /* unreachable, but keeps compiler happy */
}
