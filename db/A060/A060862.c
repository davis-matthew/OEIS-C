/*
 * A060862  a(n) = 0 if n is deficient, 1 if n is abundant, 2 if n is perfect.
 * https://oeis.org/A060862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n is deficient, 1 if n is abundant, 2 if n is
 *            perfect
 * Range:     u8 n <= 2824, u16 n <= 8796, u32 n <= 8796, u64 n <= 8796,
 *            u128 n <= 8796, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A060862 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle n = 1 explicitly – 1 is deficient */
    if (n == 1) return 0;

    idx_t tmp      = n;       /* number to factor */
    seq_t sigma    = 1;       /* product of (p^{a+1}-1)/(p-1) for each prime factor */

    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            seq_t term   = 1; /* 1 + p + p^2 + ... + p^a */
            seq_t p_pow  = 1; /* current p^k */

            do
            {
                tmp  /= p;
                p_pow*= (seq_t)p;
                term += p_pow;
            } while (tmp % p == 0);

            sigma *= term;
        }
    }

    if (tmp > 1)
    {
        /* tmp is prime now */
        sigma *= (seq_t)(1 + tmp);
    }

    /* sum of proper divisors = sigma(n) - n */
    seq_t proper = sigma - (seq_t)n;

    if (proper == (seq_t)n)    return 2; /* perfect */
    else if (proper < (seq_t)n) return 0; /* deficient */
    else                       return 1; /* abundant */
}
