/*
 * A215865  Number of simple labeled graphs on n+5 nodes with exactly n
 *          connected components that are trees or cycles.
 * https://oeis.org/A215865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+5,6)*(3*n^4+130*n^3+1565*n^2+7006*n+12992)/16
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 11, u64 n <= 187, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate when seq_t is a 64‑bit unsigned type. */
    if (sizeof(seq_t) == sizeof(unsigned long long)) {
        __int128 xn = n;
        __int128 x2 = xn * xn;
        __int128 x3 = x2 * xn;
        __int128 x4 = x3 * xn;
        __int128 poly = 3 * x4 + 130 * x3 + 1565 * x2 + 7006 * xn + 12992;
        __int128 binom_val = (__int128)(n + 5) * (n + 4) * (n + 3) *
                              (n + 2) * (n + 1) * n;
        binom_val /= 720;                    /* 6! = 720 */
        __int128 prod = binom_val * poly;
        return (seq_t)(prod >> 4);            /* divide by 16 */
    }

    /* For arbitrary‑precision or 128‑bit seq_t, use its own arithmetic. */
    seq_t x   = (seq_t)n;
    seq_t x2  = x * x;
    seq_t x3  = x2 * x;
    seq_t x4  = x3 * x;
    seq_t poly = 3 * x4 + 130 * x3 + 1565 * x2 + 7006 * x + 12992;
    seq_t bc   = binom(n + 5, 6);         /* C(n+5,6) */
    seq_t prod = bc * poly;
    return prod >> 4;                     /* integer division by 16 */
}
