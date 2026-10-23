/*
 * A018911  Expansion of 1/((1-4x)(1-5x)(1-9x)).
 * https://oeis.org/A018911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 101*a(n-2) + 180*a(n-3) (n > 2) with initial
 *            values a(0)=1, a(1)=18, a(2)=223
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
constexpr bool seq_t_is_integral = std::is_integral<seq_t>::value;
#endif

seq_t a(idx_t n)
{
    /* Direct return for the first three terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)223;

    /* Variables holding a(n-3), a(n-2), a(n-1). */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)18;  /* a(1) */
    seq_t a_nm1 = (seq_t)223; /* a(2) */
    seq_t a_n;                /* a(i) being computed */

    for (idx_t i = 3; i <= n; ++i) {
#ifdef __cplusplus
        if constexpr (seq_t_is_integral) {
            /* Use 128‑bit integers to avoid overflow when seq_t is integral. */
            signed __int128 t =
                (signed __int128)a_nm1 * 18
              + (signed __int128)a_nm2 * (-101)
              + (signed __int128)a_nm3 * 180;
            a_n = (seq_t)t;
        } else {
            /* Let the arbitrary‑precision class handle the arithmetic. */
            a_n = a_nm1 * 18 - a_nm2 * 101 + a_nm3 * 180;
        }
#else
        /* In C we always use a 128‑bit signed integer. */
        signed __int128 t =
            (signed __int128)a_nm1 * 18
          + (signed __int128)a_nm2 * (-101)
          + (signed __int128)a_nm3 * 180;
        a_n = (seq_t)t;
#endif
        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
