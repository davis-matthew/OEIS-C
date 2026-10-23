/*
 * A045890  Catafusenes (see reference for precise definition).
 * https://oeis.org/A045890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+2)*(n+3)*a(n) = 2*(n+2)*(3*n+4)*a(n-1) -
 *            5*(n-2)*(n+3)*a(n-2), with a(0)=1, a(1)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 56,
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
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)3;   /* a(1) */
    seq_t a_curr = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i)
    {
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value)
        {
            /* use 128‑bit intermediates for safety */
            unsigned __int128 num1 = (unsigned __int128)a_prev1 *
                                    (unsigned __int128)(2 * (3 * i + 4));
            seq_t term1 = (seq_t)(num1 / (i + 3));

            unsigned __int128 num2 = (unsigned __int128)a_prev2 *
                                    (unsigned __int128)(5 * (i - 2));
            seq_t term2 = (seq_t)(num2 / (i + 2));

            a_curr = term1 - term2;
        }
        else
        {
            /* big‑integer seq_t : rely on its native operations */
            seq_t term1 = (a_prev1 * (seq_t)(2 * (3 * i + 4))) / (seq_t)(i + 3);
            seq_t term2 = (a_prev2 * (seq_t)(5 * (i - 2)))   / (seq_t)(i + 2);
            a_curr = term1 - term2;
        }
#else
        /* C compile path – 128‑bit intermediates are available */
        unsigned __int128 num1 = (unsigned __int128)a_prev1 *
                                (unsigned __int128)(2 * (3 * i + 4));
        seq_t term1 = (seq_t)(num1 / (i + 3));

        unsigned __int128 num2 = (unsigned __int128)a_prev2 *
                                (unsigned __int128)(5 * (i - 2));
        seq_t term2 = (seq_t)(num2 / (i + 2));

        a_curr = term1 - term2;
#endif
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
