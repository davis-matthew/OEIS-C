/*
 * A059909  a(n) = |{m : multiplicative order of n mod m = 4}|.
 * https://oeis.org/A059909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n^4-1) - tau(n^2-1) (tau = divisor count)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

/* Count divisors of unsigned 64‑bit integer x using factor_uint64.
   The factor_uint64 routine returns the distinct prime factors in fac[]
   and their exponents in exp[], with cnt entries. */
static seq_t tau_uint64(uint64_t x)
{
    if (x == 1) return (seq_t)1;  /* tau(1) = 1 */
    uint64_t fac[20];
    int    exp[20];
    int    cnt = 0;
    factor_uint64(x, fac, exp, &cnt);
    seq_t res = 1;
    for (int i = 0; i < cnt; ++i)
        res *= (seq_t)(exp[i] + 1);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined, return 0 */

    /* For n == 1 the formula involves tau(0), which is not defined.
       The OEIS entry lists a(1) = 0. */
    if ((idx_t)1 == n)            /* cast to avoid warning on mixed types */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t n2 = nn * nn;        /* n^2  (fits in 64‑bit for n <= 10^4) */
    uint64_t n4 = n2 * n2;        /* n^4  (fits in 64‑bit for n <= 10^4) */

    seq_t t1 = tau_uint64(n4 - 1);
    seq_t t2 = tau_uint64(n2 - 1);

    return t1 - t2;
}
