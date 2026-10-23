/*
 * A176789  a(n) = (0!! - 1!! + 2!! - 3!! + ... + (-1)^(n-1)*(n-1)!!) mod n.
 * https://oeis.org/A176789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^k * k!! mod n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* we compute everything modulo n using a 64‑bit accumulator
       (idx_t is at least 64 bits).  The result is cast to seq_t
       for the return value.  This works for all seq_t sizes. */
    if (n == 0) return 0;          /* undefined, but harmless */

    idx_t mod = n;
    idx_t sum_mod = 0;              /* will stay < n */
    idx_t d_prev2 = 1 % mod;        /* d_0 */
    idx_t d_prev1 = 1 % mod;        /* d_1 */

    for (idx_t k = 0; k < n; ++k)
    {
        idx_t d;
        if (k == 0)
            d = d_prev2;            /* d0 = 1 */
        else if (k == 1)
            d = d_prev1;            /* d1 = 1 */
        else
        {
            /* d_k = k * d_{k-2} (mod n) */
            d = (k * d_prev2) % mod;
        }

        /* add with alternating sign */
        if ((k & 1) == 0) /* even k */
        {
            sum_mod += d;
        }
        else /* odd k, subtract */
        {
            sum_mod += mod - d;     /* equivalent to -d mod n */
        }
        sum_mod %= mod;

        /* shift previous double factorials for next iteration */
        if (k >= 2)
        {
            d_prev2 = d_prev1;
            d_prev1 = d;
        }
    }

    return (seq_t)sum_mod;           /* convert to seq_t for the caller */
}
