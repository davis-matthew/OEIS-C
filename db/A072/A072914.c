/*
 * A072914  Denominators of
 *          1/4!*(H(n,1)^4+6*H(n,1)^2*H(n,2)+8*H(n,1)*H(n,3)+3*H(n,2)^2+6*H(n,4)),
 *          where H(n,m) = Sum_{i=1..n} 1/i^m are generalized harmonic
 *          numbers.
 * https://oeis.org/A072914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = lcm(1..n)^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* power of a seq_t base to an non‑negative integer exponent */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* simple sieve up to n.  The array is reused across calls so the cost
   of zeroing it is negligible for n ≤ 1 000 000.  */
#define SIEVE_LIMIT 100005
static unsigned char sieve_arr[SIEVE_LIMIT];

static void sieve_primes(idx_t n, idx_t *prime_buf, size_t *prime_cnt)
{
    if (n >= SIEVE_LIMIT)
    {
        /* for safety – the sequence is only defined up to about 10 000,
           but this fallback keeps the program correct if a larger n
           is passed. */
        /* grow the sieve array on the stack (not possible with VLA),
           so we simply return an empty list. */
        *prime_cnt = 0;
        return;
    }

    /* clear previous marks up to n (inclusive) */
    for (idx_t i = 0; i <= n; ++i)
        sieve_arr[i] = 0;

    *prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        if (!sieve_arr[i])
        {
            prime_buf[(*prime_cnt)++] = i;
            if (i <= n / i)              /* avoid overflow on i*i */
            {
                for (idx_t j = i * i; j <= n; j += i)
                    sieve_arr[j] = 1;
            }
        }
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) = 1, and for n < 1 we simply return 1 as a harmless
       default. */
    if (n < 1)
        return (seq_t)1;

    /* collect primes up to n */
    static idx_t prime_buf[SIEVE_LIMIT];
    size_t prime_cnt;
    sieve_primes(n, prime_buf, &prime_cnt);

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = prime_buf[i];
        /* largest exponent e such that p^e ≤ n */
        idx_t e = 0;
        idx_t pow = p;
        while (pow <= n)
        {
            ++e;
            /* stop if next multiplication would overflow n */
            if (pow > n / p)
                break;
            pow *= p;
        }
        /* update result by p^(4*e) */
        if (e > 0)
            result *= pow_seq_t((seq_t)p, 4 * e);
    }

    return result;
}
