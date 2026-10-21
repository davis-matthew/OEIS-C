/*
 * A236399  Left factorial !p, where p = prime(n).
 * https://oeis.org/A236399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0}^{prime(n)-1} k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"   /* unsigned uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    if (n < 1)   /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Get the n‑th prime p = prime(n) */
    uint64_t p = (uint64_t)get_prime(n);   /* prime(1)=2, prime(2)=3, … */

    /* Compute sum_{k=0}^{p-1} k! sequentially. */
    seq_t sum   = (seq_t)1;   /* 0! = 1 */
    seq_t fact  = (seq_t)1;   /* will hold k! during the loop */

    /* If p == 0 or 1 the loop body is skipped, but p is always ≥ 2 here */
    for (idx_t k = 1; k < p; ++k) {
        fact *= (seq_t)k;     /* k! = (k-1)! * k */
        sum  += fact;        /* add it to the sum */
    }

    return sum;
}
