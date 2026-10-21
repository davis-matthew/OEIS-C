/*
 * A089911  a(n) = Fibonacci(n) mod 12.
 * https://oeis.org/A089911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear, easy
 * Formula:   a(n) = Fibonacci(n) mod 12 (recurrence: a(0)=0, a(1)=1,
 *            a(n)=a(n-1)+a(n-2) mod 12)
 * Range:     u8 n <= 1199, u16 n <= 1199, u32 n <= 1199, u64 n <= 1199,
 *            u128 n <= 1199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for fixed‑width integer types, if needed */

seq_t a(idx_t n)
{
    /* Modulus for Fibonacci numbers */
    const seq_t MOD = 12;

    /* F(0) = 0, F(1) = 1 */
    seq_t f0 = 0;
    seq_t f1 = 1;

    /* Handle n = 0 immediately */
    if (n == 0)
        return f0;

    /* Find the position of the most significant set bit in n */
    idx_t bit = 63;                                 /* 63: enough for 64‑bit idx_t */
    while ((n >> bit) == 0 && bit > 0)
        --bit;

    for (idx_t i = bit; ; --i)
    {
        /* Fast‑doubling step: compute (F(2k), F(2k+1)) from (F(k), F(k+1)) */
        seq_t d = (f0 * (((seq_t)2 * f1) % MOD + MOD - f0) % MOD) % MOD; /* F(2k) */
        seq_t e = (f0 * f0 + f1 * f1) % MOD; /* F(2k+1) */

        if ( (n >> i) & 1 )
        {
            /* If current bit is 1: we need F(2k+1) and F(2k+2) */
            f0 = e;
            f1 = (d + e) % MOD;
        }
        else
        {
            /* If current bit is 0: we stay at (F(2k), F(2k+1)) */
            f0 = d;
            f1 = e;
        }

        if (i == 0)
            break;
    }

    return f0;
}
