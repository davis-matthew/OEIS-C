/*
 * A025751  6th-order Patalan numbers (generalization of Catalan numbers).
 * https://oeis.org/A025751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * 6 * (6*n – 7) / n, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef NO_IMPLEMENTATION
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Handle the initial terms. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i)
    {
#ifdef __cplusplus
        /* For integral seq_t types we use 128‑bit arithmetic to avoid overflow. */
        if constexpr (std::is_integral<seq_t>::value) {
            unsigned __int128 tmp =
                (unsigned __int128)res * (unsigned long long)6 *
                (unsigned long long)(6 * i - 7);
            tmp /= (unsigned __int128)i;
            res = (seq_t)tmp;
        } else {
            /* For arbitrary‑precision seq_t types the overloaded operators are
               sufficient. */
            seq_t tmp = res * (seq_t)6 * (seq_t)(6 * i - 7);
            tmp /= (seq_t)i;
            res = tmp;
        }
#else
        /* In plain C we always use a 128‑bit intermediate for safety. */
        unsigned __int128 tmp =
            (unsigned __int128)res * (unsigned long long)6 *
            (unsigned long long)(6 * i - 7);
        tmp /= (unsigned __int128)i;
        res = (seq_t)tmp;
#endif
    }

    return res;
}
#endif
