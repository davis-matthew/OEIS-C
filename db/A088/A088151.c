/*
 * A088151  Value of n-th digit in ternary representation of n^n.
 * https://oeis.org/A088151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n^n / 3^n) mod 3
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* 0^0 is defined as 1 */
    
    size_t len = (size_t)n + 1;           /* need digits 0 … n */
    seq_t *d = (seq_t *)malloc(len * sizeof(seq_t));
    if (!d) return (seq_t)0;              /* out‑of‑memory fallback */
    
    for (size_t i = 0; i < len; ++i) d[i] = 0;
    d[0] = 1;                              /* start with 1 */
    
    for (idx_t step = 1; step <= n; ++step)
    {
        unsigned long long carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            unsigned long long temp = carry + (unsigned long long)d[i] * (unsigned long long)n;
            d[i] = (seq_t)(temp % 3);
            carry  = temp / 3;
        }
        /* any remaining carry is from higher digits and can be discarded */
    }
    
    seq_t result = d[n];
    free(d);
    return result;
}
