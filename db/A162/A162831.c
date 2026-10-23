/*
 * A162831  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=29, a(2)=812, a(3)=22330,
 *            a(n)=27*a(n‑1)+27*a(n‑2)−378*a(n‑3) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(__cplusplus)
#  define MUL_SMALL(a,c) ((a)*(c))
#else
#  define MUL_SMALL(a,c)                                          \
   _Generic((a),                                                   \
     uint8_t:  (uint8_t)((__uint128_t)(a) * (c)),                 \
     uint16_t: (uint16_t)((__uint128_t)(a) * (c)),                \
     uint32_t: (uint32_t)((__uint128_t)(a) * (c)),                \
     uint64_t: (uint64_t)((__uint128_t)(a) * (c)),                \
     unsigned __int128: ((__uint128_t)(a) * (c)),                 \
     default:  ((seq_t)((__uint128_t)(a) * (c)))                 \
   )
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)812;
    if (n == 3) return (seq_t)22330;

    seq_t p1 = (seq_t)29;   /* a(n-3) */
    seq_t p2 = (seq_t)812;  /* a(n-2) */
    seq_t p3 = (seq_t)22330;/* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t cur = MUL_SMALL(p3, 27) + MUL_SMALL(p2, 27) - MUL_SMALL(p1, 378);
        p1 = p2;
        p2 = p3;
        p3 = cur;
    }
    return p3;
}
