/*
 * A227499  Number of the Lipschitz quaternions in a reduced system modulo n.
 * https://oeis.org/A227499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(2^e)=2^(4e-1),
 *            a(p^e)=p^(4e-3)*(p-1)^2*(p+1) for odd prime p
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_mul.h"        /* for seq_mul(seq_t,seq_t) */

/* Helper to compute small ^ exp with seq_t result */
static seq_t seq_pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t curr   = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result = seq_mul(result, curr);
        curr = seq_mul(curr, curr);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* trial division factorisation */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p != 0) continue;
        idx_t exp = 0;
        while (m % p == 0)
        {
            m /= p;
            ++exp;
        }

        seq_t contrib;
        if (p == 2)
        {
            /* a(2^e)=2^(4e-1) */
            contrib = seq_pow_uint(2, 4 * exp - 1);
        }
        else
        {
            /* a(p^e)=p^(4e-3)*(p-1)^2*(p+1) */
            contrib = seq_pow_uint(p, 4 * exp - 3);
            seq_t t1 = seq_pow_uint(p - 1, 2);
            seq_t t2 = seq_pow_uint(p + 1, 1);
            contrib = seq_mul(contrib, seq_mul(t1, t2));
        }
        result = seq_mul(result, contrib);
    }

    /* remaining prime factor (> sqrt) if any */
    if (m > 1)
    {
        /* exp = 1 for remaining factor */
        seq_t contrib;
        if (m == 2)
            contrib = seq_pow_uint(2, 4 * 1 - 1);          /* 2^(4-1)=8 */
        else
        {
            contrib = seq_pow_uint(m, 4 * 1 - 3);          /* p^1 */
            seq_t t1 = seq_pow_uint(m - 1, 2);
            seq_t t2 = seq_pow_uint(m + 1, 1);
            contrib = seq_mul(contrib, seq_mul(t1, t2));
        }
        result = seq_mul(result, contrib);
    }

    return result;
}
