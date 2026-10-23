/*
 * A172052  a(n)=abs(A171696(n)-A002822(n)).
 * https://oeis.org/A172052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A172052: a(n) = |A171696(n) – A002822(n)|
   A171696(n) – n-th non‑negative integer k with both 6k-1 and 6k+1 composite.
   A002822(n) – n‑th integer m with both 6m-1 and 6m+1 prime (twin primes). */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t cnt1 = 0, cnt2 = 0;   /* counters for the two sequences */
    seq_t x = 0, y = 0;         /* the nth terms once found */
    idx_t k = 0;                /* current integer to test */

    while (cnt1 < n || cnt2 < n)
    {
        if (k == 0)
        {
            /* 6*0-1 = -1 and 6*0+1 = 1 are not prime – count for A171696 only. */
            if (cnt1 < n) { x = (seq_t)k; ++cnt1; }
        }
        else
        {
            idx_t p1 = 6 * k - 1;
            idx_t p2 = 6 * k + 1;

            bool prime1 = is_prime(p1);
            bool prime2 = is_prime(p2);

            /* A171696: both composite */
            if (!prime1 && !prime2)
            {
                if (cnt1 < n) { x = (seq_t)k; ++cnt1; }
            }

            /* A002822: both prime */
            if (prime1 && prime2)
            {
                if (cnt2 < n) { y = (seq_t)k; ++cnt2; }
            }
        }
        ++k;
    }

    return (x > y) ? (x - y) : (y - x);
}
