/*
 * A179964  Number of permutations p of [2*n+6] such that |p(i)-p(i-1)| > n
 *          for i>1.
 * https://oeis.org/A179964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given for n ≤ 5 by the table below. For n ≥ 6 it
 *            satisfies the linear recurrence a(n) = 22*a(n-1) – 157*a(n-2) +
 *            360*a(n-3) (n ≥ 6), where a(0)=720, a(1)=5242, a(2)=44304,
 *            a(3)=399848, a(4)=3661824, a(5)=33461568
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Always include the size header for integer types */
#include <stddef.h>

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 5 */
    static const seq_t init[6] = {
        (seq_t)720,
        (seq_t)5242,
        (seq_t)44304,
        (seq_t)399848,
        (seq_t)3661824,
        (seq_t)33461568
    };

    if (n < 6u)
        return init[n];

    /* initialise with a(3), a(4), a(5) */
    seq_t prev3 = init[3];   /* a(n-3) */
    seq_t prev2 = init[4];   /* a(n-2) */
    seq_t prev1 = init[5];   /* a(n-1) */
    seq_t next  = prev1;     /* placeholder */

    for (idx_t i = 6u; i <= n; ++i)
    {
#if defined(__cplusplus)
        /* In C++ use 128‑bit arithmetic only for integral
           seq_t types; otherwise rely on seq_t arithmetic. */
        if constexpr (std::is_integral_v<seq_t>) {
            unsigned __int128 temp =
                (unsigned __int128)prev1 * 22u
              - (unsigned __int128)prev2 * 157u
              + (unsigned __int128)prev3 * 360u;
            next = (seq_t)temp;
        } else {
            /* seq_t is a big‑integer type */
            next = (seq_t)22 * prev1
                 - (seq_t)157 * prev2
                 + (seq_t)360 * prev3;
        }
#else
        /* In plain C we rely on 128‑bit intermediate arithmetic. */
        unsigned __int128 temp =
            (unsigned __int128)prev1 * 22u
          - (unsigned __int128)prev2 * 157u
          + (unsigned __int128)prev3 * 360u;
        next = (seq_t)temp;
#endif
        /* shift the window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
