/*
 * A092403  a(n) = sigma(n) + sigma(n+1).
 * https://oeis.org/A092403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n) + σ(n+1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute σ(m) – sum of the positive divisors of m */
static seq_t sigma(idx_t m)
{
    seq_t result = 1;          /* product of divisor‑sum factors */
    idx_t   temp   = m;        /* copy for factorisation */

    /* trial‑division by all possible primes */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* contribution of the prime power p^e  */
            seq_t term_sum = 1;   /* sum_{k=0..e} p^k */
            seq_t power    = 1;
            while (temp % p == 0)
            {
                temp /= p;
                power *= (seq_t)p;
                term_sum += power;
            }
            result *= term_sum;
        }
    }

    /* possible remaining prime factor */
    if (temp > 1)
        result *= (seq_t)(1 + temp);

    return result;
}

/* OEIS implementation for A092403: a(n) = σ(n) + σ(n+1) */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – behaviour for n<1 is irrelevant */
        return (seq_t)0;

    seq_t s1 = sigma(n);
    seq_t s2 = sigma(n + 1);
    return s1 + s2;
}
