/*
 * A053336  a(n) contains n digits (either '5' or '6') and is divisible by
 *          2^n.
 * https://oeis.org/A053336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(6 - [(a(n-1)/2^(n-1)) mod 2])
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(1) = 6.   (offset 1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; return 0 for safety */
        return (seq_t)0;

    seq_t term  = (seq_t)6;  /* a(1) */
    seq_t pow10 = (seq_t)10; /* 10^1, the multiplier for the next digit */

    /* Build a(2) … a(n) iteratively.  At step k (1‑based) we already have a(k). */
    for (idx_t k = 1; k < n; ++k)
    {
        /* The new most‑significant digit depends on bit (k) of the current term: */
        seq_t bit   = (term >> k) & (seq_t)1;      /* 0 or 1 */
        seq_t digit = 6 - bit;                     /* 5 or 6     */

        term  = pow10 * digit + term;   /* prepend the new digit   */
        pow10 = pow10 * (seq_t)10;      /* 10^(k+1) for the next step */
    }

    return term;
}
