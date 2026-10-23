/*
 * A152190  a(n)=2*Product[(1 + 4*Cos[k*Pi/n]^2)*(1 + 4*Sin[k*Pi/n]^2), {k, 1,
 *          Floor[(n - 1)/2]}] - Product[(1 + 4*Sin[k*Pi/n]^2), {k, 1,
 *          Floor[(n - 1)/2]}].
 * https://oeis.org/A152190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S_n * (2*F_n - 1), where F_n is the Fibonacci number, S_n
 *            = F_n if n is even, S_n = L_n (Lucas number) if n is odd. Lucas
 *            number L_n satisfies L_n = 2*F_{n+1} - F_n
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   fibpair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t, seq_t are defined by the build */
#include "../utils/fibpair.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                       /* a(0) is defined to be 1 */

    seq_t F_n, F_np1;
    fibpair(n, &F_n, &F_np1);            /* compute F_n and F_{n+1} */

    seq_t twoF_n          = F_n + F_n;    /* 2*F_n */
    seq_t twoF_n_minus_1  = twoF_n - 1;   /* 2*F_n - 1 */

    seq_t S_n;                           /* S_n = F_n or Lucas(n) */
    if ((n & 1) == 0)                    /* n even */
        S_n = F_n;
    else {                               /* n odd: Lucas(n) = 2*F_{n+1} - F_n */
        seq_t twoF_np1 = F_np1 + F_np1;
        S_n = twoF_np1 - F_n;
    }

    return S_n * twoF_n_minus_1;         /* a(n) = S_n * (2*F_n - 1) */
}
