/*
 * A036698  a(n) is the number of Gaussian integers z=a+bi satisfying |z|<=n,
 *          a>0, b>=0.
 * https://oeis.org/A036698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{a=1}^{n} (⌊√(n² – a²)⌋ + 1), i.e. number of Gaussian
 *            integers a+bi with |a+bi| ≤ n, a > 0 and b ≥ 0
 * Range:     u8 n <= 18, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    idx_t bmax = n;                       /* current maximum b */
    __int128 n2 = (__int128)n * n;        /* n² in 128‑bit */

    for (idx_t a = 1; a <= n; ++a) {
        __int128 arest = (__int128)a * a; /* a² */

        /* Decrease bmax until a² + bmax² ≤ n² */
        while (bmax > 0) {
            __int128 sum = arest + (__int128)bmax * bmax;
            if (sum <= n2)
                break;
            --bmax;
        }

        result += (seq_t)(bmax + 1);      /* b = 0 … bmax */
    }

    return result;
}
