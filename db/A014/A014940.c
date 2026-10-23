/*
 * A014940  a(1)=1, a(n) = n*22^(n-1) + a(n-1).
 * https://oeis.org/A014940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44*a(n-1) - 484*a(n-2) + 1, a(1)=1, a(2)=45
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Undefined for n<1 – the sequence starts at n=1. */
    if (n == 1) return 1;
    if (n == 2) return 45;

#ifdef __cplusplus
    /* C++ mode: use integral or arbitrary‑precision path. */
    if constexpr (std::is_integral<seq_t>::value) {
        /* Integral types use 128‑bit intermediates to avoid overflow. */
        seq_t a_prev2 = 1;   /* a(1) */
        seq_t a_prev1 = 45;  /* a(2) */
        seq_t a_curr;
        for (idx_t i = 3; i <= n; ++i) {
            unsigned __int128 t1 = (unsigned __int128)44 * (unsigned __int128)a_prev1;
            unsigned __int128 t2 = (unsigned __int128)484 * (unsigned __int128)a_prev2;
            unsigned __int128 tmp = t1 - t2 + 1;
            a_curr = (seq_t)tmp;
            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
        return a_prev1;
    } else {
        /* Arbitrary‑precision class: rely on its operators. */
        seq_t a_prev2 = 1;   /* a(1) */
        seq_t a_prev1 = 45;  /* a(2) */
        seq_t a_curr;
        for (idx_t i = 3; i <= n; ++i) {
            a_curr = (seq_t)44 * a_prev1;
            a_curr = a_curr - (seq_t)484 * a_prev2;
            a_curr = a_curr + 1;
            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
        return a_prev1;
    }
#else
    /* C mode: seq_t is an unsigned integer type, use 128‑bit intermediates. */
    seq_t a_prev2 = 1;   /* a(1) */
    seq_t a_prev1 = 45;  /* a(2) */
    seq_t a_curr;
    for (idx_t i = 3; i <= n; ++i) {
        unsigned __int128 t1 = (unsigned __int128)44 * (unsigned __int128)a_prev1;
        unsigned __int128 t2 = (unsigned __int128)484 * (unsigned __int128)a_prev2;
        unsigned __int128 tmp = t1 - t2 + 1;
        a_curr = (seq_t)tmp;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
#endif
}
