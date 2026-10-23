/*
 * A122369  Dimension of 5-variable non-commutative harmonics (twisted
 *          derivative). The dimension of the space of non-commutative
 *          polynomials in 5 variables which are killed by all symmetric
 *          differential operators (where for a monomial w, d_{xi} ( xi w ) =
 *          w and d_{xi} ( xj w ) = 0 for i/=j).
 * https://oeis.org/A122369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 32*a(n-2) + 37*a(n-3) - 11*a(n-4) * for n ≥
 *            4, with initial values * a(0)=1, a(1)=4, a(2)=19, a(3)=93
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
/* multiply a seq_t by a small unsigned int constant,
   using 128‑bit arithmetic when seq_t is an integral type
   to avoid overflow on the 64‑bit build. */
static inline seq_t mul_const(seq_t x, unsigned int k)
{
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((unsigned __int128)x * k);
    } else {
        return (seq_t)x * k;
    }
}
#else
/* in C the seq_t type is assumed to be an unsigned integer type,
   so ordinary multiplication is sufficient (overflow free for
   the n values that fit in that type). */
static inline seq_t mul_const(seq_t x, unsigned int k)
{
    return (seq_t)x * k;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)93;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)19;
    seq_t a3 = (seq_t)93;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = mul_const(a3, 10);
        seq_t t2 = mul_const(a2, 32);
        seq_t t3 = mul_const(a1, 37);
        seq_t t4 = mul_const(a0, 11);
        next = t1 - t2 + t3 - t4;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
