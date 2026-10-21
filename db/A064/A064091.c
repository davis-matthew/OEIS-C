/*
 * A064091  Generalized Catalan numbers C(8; n).
 * https://oeis.org/A064091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((223*n-336)*a(n-1) + 16*(2*n-3)*a(n-2)) / (7*n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (recurrence obtained from OEIS entry) */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t a_nm2 = 1; /* a(0) */
    seq_t a_nm1 = 1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            unsigned __int128 term1 = (unsigned __int128)(223*i-336) * (unsigned __int128)a_nm1;
            unsigned __int128 term2 = (unsigned __int128)16 * (unsigned __int128)(2*i-3) * (unsigned __int128)a_nm2;
            unsigned __int128 num   = term1 + term2;
            seq_t ai = (seq_t)(num / ((unsigned __int128)7*i));
            a_nm2 = a_nm1;
            a_nm1 = ai;
        } else {
            seq_t term1 = ((seq_t)(223*i-336)) * a_nm1;
            seq_t term2 = ((seq_t)16 * (seq_t)(2*i-3)) * a_nm2;
            seq_t num   = term1 + term2;
            seq_t ai    = num / ((seq_t)7*i);
            a_nm2 = a_nm1;
            a_nm1 = ai;
        }
#else
        unsigned __int128 term1 = (unsigned __int128)(223*i-336) * (unsigned __int128)a_nm1;
        unsigned __int128 term2 = (unsigned __int128)16 * (unsigned __int128)(2*i-3) * (unsigned __int128)a_nm2;
        unsigned __int128 num   = term1 + term2;
        seq_t ai = (seq_t)(num / ((unsigned __int128)7*i));
        a_nm2 = a_nm1;
        a_nm1 = ai;
#endif
    }
    return a_nm1;
}
