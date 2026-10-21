/*
 * A090619  Exponent of highest power of 12 dividing n!.
 * https://oeis.org/A090619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min( floor(v2(n!)/2), v3(n!) ) where v_p(n!) = sum_{i >=
 *            1} floor(n/p^i) is Legendre's exponent of prime p in n!
 * Range:     u8 n <= 514, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t legendre_exp(idx_t n, idx_t p)
{
    seq_t e = 0;
    for (idx_t div = p; div <= n; div *= p)
    {
        e += (seq_t)(n / div);
        /* guard against overflow of div: if div > n/p, next div will overflow or exceed n */
        if (div > n / p) break;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* v2(n!) and v3(n!) */
    seq_t e2 = legendre_exp(n, 2);
    seq_t e3 = legendre_exp(n, 3);
    seq_t v2half = e2 / 2;      /* floor division */
    return (v2half < e3) ? v2half : e3;
}
