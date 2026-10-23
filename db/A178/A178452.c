/*
 * A178452  Partial sums of floor(2^n/5).
 * https://oeis.org/A178452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Partial sums of floor(2^n/5)  
    a(n) = Σ_{k=1..n} ⌊2^k/5⌋
    computed by the recurrence
        q_0 = 0, r_0 = 1   (r_k = 2^k mod 5)
        q_{k+1} = 2*q_k + (r_k ≥ 3 ? 1 : 0)
        r_{k+1} = (2 * r_k) mod 5
    Then a(n) = Σ_{k=1..n} q_k.  */
#include <stddef.h>   /* for size_t, idx_t and seq_t are provided by the build */

seq_t a(idx_t n)
{
    seq_t sum = 0;      /* running total of the partial sums   */
    seq_t q   = 0;      /* current value floor(2^k/5)          */
    unsigned int r = 1; /* current remainder 2^k mod 5          */

    for (idx_t i = 0; i < n; ++i)        /* iterate k = 0 .. n-1          */
    {
        unsigned int inc = (r >= 3) ? 1 : 0; /* floor(2*r/5) with r∈{1,2,3,4}  */
        q = q * 2 + inc;                   /* compute q_{k+1}                 */
        r = (unsigned int)((r * 2) % 5);   /* update remainder to 2^{k+1} mod 5 */
        sum += q;                          /* accumulate floor(2^{k+1}/5)      */
    }
    return sum;
}
