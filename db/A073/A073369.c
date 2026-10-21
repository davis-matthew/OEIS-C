/*
 * A073369  Let c(1)=n, e=2.71... and c(k+1)=floor(c(k)/e) if c(k) is even,
 *          c(k+1)=floor(e*c(k)) otherwise; sequence gives the smallest value
 *          a(n) such that c(a(n))=0.
 * https://oeis.org/A073369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the first index k with c_k = 0, where 
   c_1 = n and 
          c_{k+1} = floor(c_k / e) if c_k is even
          c_{k+1} = floor(e * c_k) if c_k is odd
   (e = 2.718281828459045...).
   The following code simulates this recurrence using a 128‑bit
   rational approximation of e, giving the exact integer values
   for all n that fit in seq_t.
*/

#include <stdint.h>
#include <stdbool.h>

static const unsigned long long D = 1000000000000000000ULL;   /* 10^18  */
static const unsigned long long M = 2718281828459045235ULL;   /* floor(e*D)  */
static const unsigned long long R = 367879441171442321ULL;   /* floor(D/e) */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;     /* c_1 = n */
    idx_t  step = 1;          /* index counter */

    while (cur != 0)          /* stop when we reach 0 */
    {
        if (cur & 1ULL)        /* odd: c_{k+1} = floor(e * cur) */
        {
            __int128 prod = (__int128)cur * M;
            cur = (seq_t)(prod / D);
        }
        else                  /* even: c_{k+1} = floor(cur / e) */
        {
            __int128 prod = (__int128)cur * R;
            cur = (seq_t)(prod / D);
        }
        step++;              /* advance to the next index */
    }

    return step;             /* a(n) = first index reaching 0 */
}
