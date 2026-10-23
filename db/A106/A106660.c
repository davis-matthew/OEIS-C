/*
 * A106660  A triangle with sides that are three consecutive integers has an
 *          area that is a prime after rounding. The first of the consecutive
 *          numbers gives the sequence.
 * https://oeis.org/A106660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer a such that the area of the triangle
 *            with sides a, a+1, a+2, rounded to the nearest integer, is
 *            prime. The area for sides a,a+1,a+2 is (a+1)/4 *
 *            sqrt(3*(a-1)*(a+3)). The rounded area is floor( (
 *            floor(sqrt(3*(a-1)*(a+3)*(a+1)^2)) + 2 ) / 4 ). Memory‑stateless
 *            implementati...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 24, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   isprime_big, isqrt, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/push_seq.h"
#include "../utils/isprime_big.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;      /* array of found terms           */
    static idx_t cap   = 0;          /* current capacity of array      */
    static idx_t size  = 0;          /* number of terms stored         */
    static seq_t next_a = 2;         /* next candidate a to test       */

    /* Generate terms until we have at least n of them */
    while (size < n)
    {
        seq_t a_val = next_a;                         /* candidate a           */
        /* compute R = 3*(a-1)*(a+3)*(a+1)^2 */
        seq_t R = 3;
        R *= (a_val - 1);
        R *= (a_val + 3);
        R *= (a_val + 1);
        R *= (a_val + 1);

        /* rounded area = floor((floor(sqrt(R)) + 2) / 4) */
        seq_t sqrtR = isqrt(R);
        seq_t areaRounded = (sqrtR + 2) / 4;

        /* primality test on the rounded area */
        if (isprime_big(areaRounded))
        {
            push_seq(&cache, &cap, &size, a_val);
        }

        ++next_a;   /* advance to the next candidate */
    }

    return cache[n - 1];
}
