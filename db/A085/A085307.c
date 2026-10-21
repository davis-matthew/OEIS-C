/*
 * A085307  a(1) = 1; for n > 1, concatenate distinct prime factors of n in
 *          decreasing order.
 * https://oeis.org/A085307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = concatenation of the distinct prime factors of n sorted
 *            in decreasing order (decimal representation)
 * Range:     u8 n <= 29, u16 n <= 857, u32 n <= 9723, u64 n <= 9723,
 *            u128 n <= 9723, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int digits_uint64(uint64_t x)
{
    int cnt = 0;
    do
    {
        ++cnt;
        x /= 10;
    } while (x);
    return cnt;
}

static seq_t pow10_seq(size_t e)
{
    seq_t r = 1;
    for (size_t i = 0; i < e; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1U)            /* a(1) = 1 */
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];     /* ample space for distinct primes of a 64‑bit number */
    size_t   pcnt = 0;
    uint64_t temp = m;

    /* factorise m, keeping only distinct prime factors (ascending order) */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[pcnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        primes[pcnt++] = temp;

    /* concatenate primes in decreasing order */
    seq_t result = 0;
    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p   = primes[pcnt - i - 1];          /* reverse the list */
        int      digs = digits_uint64(p);
        seq_t    pow10 = pow10_seq((size_t)digs);

        result = result * pow10 + (seq_t)p;
    }

    return result;
}
