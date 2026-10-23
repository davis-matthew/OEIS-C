/*
 * A104589  a(1)=1. a(n) = a(n-1) + (sum of terms, from among terms a(1)
 *          through a(n-1), which are prime or 1).
 * https://oeis.org/A104589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula:  a(1)=1, a(2)=2.  
   For n>=3: if a(n-1) is prime then a(n)=3*a(n-1)-a(n-2)  
   else a(n)=2*a(n-1)-a(n-2).  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)1; /* a(1) */
    seq_t prev1 = (seq_t)2; /* a(2) */
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        bool prime = is_prime((uint64_t)prev1);
        if (prime)
            curr = (seq_t)3 * prev1 - prev2;
        else
            curr = (seq_t)2 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
