/*
 * A098270  a(n) = 2^n*P_n(5), 2^n times the Legendre polynomial of order n at
 *          5.
 * https://oeis.org/A098270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   n*a(n) +10*(1-2*n)*a(n-1) +4*(n-1)*a(n-2) = 0, with a(0)=1,
 *            a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#if defined(__cplusplus)
#  include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        /* seq_t is an unsigned integer type (uint8_t … uint64_t or
           unsigned __int128).  Use 128‑bit intermediate to avoid overflow
           in the 64‑bit case. */
        seq_t a0 = (seq_t)1, a1 = (seq_t)10, an;
        for (idx_t i = 2; i <= n; ++i) {
            __uint128_t term1 = (__uint128_t)a1 * (20*i - 10);
            __uint128_t term2 = (__uint128_t)a0 * (4*(i-1));
            __uint128_t num   = term1 - term2;
            an = (seq_t)(num / i);
            a0 = a1;
            a1 = an;
        }
        return a1;
    } else {
        /* seq_t is an arbitrary‑precision integer class.  Multiplication
           and division are performed using the class’s operators. */
        seq_t a0 = (seq_t)1, a1 = (seq_t)10, an;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t term1 = a1 * (seq_t)(20*i - 10);
            seq_t term2 = a0 * (seq_t)(4*(i-1));
            seq_t num   = term1 - term2;
            an = num / i;
            a0 = a1;
            a1 = an;
        }
        return a1;
    }
#else
    /* C11 implementation using 128‑bit intermediate type. */
    seq_t a0 = (seq_t)1, a1 = (seq_t)10, an;
    for (idx_t i = 2; i <= n; ++i) {
        __uint128_t term1 = (__uint128_t)a1 * (20*i - 10);
        __uint128_t term2 = (__uint128_t)a0 * (4*(i-1));
        __uint128_t num   = term1 - term2;
        an = (seq_t)(num / i);
        a0 = a1;
        a1 = an;
    }
    return a1;
#endif
}
