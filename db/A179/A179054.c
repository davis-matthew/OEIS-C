/*
 * A179054  a(n) = (1^k + 2^k + ... + n^k) modulo (n+2), where k is any odd
 *          integer greater than or equal to 3.
 * https://oeis.org/A179054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(n/4)+2 if n%4==0, else a(n)=1
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (derived from the definition a(n) = (Σ i^k  for 1≤i≤n) mod (n+2) with k odd ≥3) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for the sequence, but prevents overflow */

    if ((n & 3) == 0)              /* n divisible by 4 */
    {
        seq_t m = (seq_t)(n >> 2); /* m = n/4 */
        return m * 2 + 2;
    }
    else
    {
        return 1;
    }
}
