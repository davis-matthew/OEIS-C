/*
 * A027616  Number of permutations of n elements containing a 2-cycle.
 * https://oeis.org/A027616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1 - Sum_{k=0..floor(n/2)} (-1)^k / (2^k * k!))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(0)=0, a(1)=0 */
    
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;
    
    /* Compute S(n) = Sum_{k=0..floor(n/2)} (-1)^k * n!/(2^k * k!)  */
    seq_t sum = 0;
    seq_t term = fact;                  /* term for k = 0 */
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        if (k > 0)                      /* term_{k} = term_{k-1} / (2*k) */
            term /= ((seq_t)2 * (seq_t)k);
        
        if ((k & 1) == 0)  sum += term; /* even k : + */
        else                sum -= term; /* odd  k : - */
    }
    
    /* a(n) = n! - S(n) = fact - sum */
    return fact - sum;
}
