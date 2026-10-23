/*
 * A016142  Expansion of 1/((1-3*x)*(1-9*x)).
 * https://oeis.org/A016142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(9^(n+1) - 3^(n+1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=12, a(n)=12*a(n-1) - 27*a(n-2) */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)12;  /* a(1) */
    seq_t an = a1;

    for (idx_t i = 2; i <= n; ++i)
    {
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value)
        {
            /* Use 128‑bit intermediate to avoid overflow for fixed‑size integer types */
            __int128 t1 = (__int128)a1 * 12;
            __int128 t2 = (__int128)a0 * 27;
            an = (seq_t)(t1 - t2);
        }
        else
        {
            /* For arbitrary‑precision types rely on operator overloading */
            an = a1 * 12 - a0 * 27;
        }
#else
        /* C mode – seq_t is an integer type; use 128‑bit intermediate */
        __int128 t1 = (__int128)a1 * 12;
        __int128 t2 = (__int128)a0 * 27;
        an = (seq_t)(t1 - t2);
#endif
        a0 = a1;
        a1 = an;
    }

    return an;
}
