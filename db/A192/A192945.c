/*
 * A192945  G.f. A(x) satisfies A(x) = 1 + x*Sum_{n>=0} (A(x)^2 - 1)^n.
 * https://oeis.org/A192945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2. For n ≥ 3, 8*(n-1)*n*a(n) =
 *            34*(n-1)*(2*n-3)*a(n-1) + 3*(3*n-7)*(3*n-5)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;          /* a(0) */
    seq_t a_nm1 = (seq_t)2;          /* a(1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 3u; i <= n; ++i)
    {
        idx_t k = i - 1u;            /* (n-1) in the recurrence */

#ifdef __cplusplus
        /* C++ build – seq_t may be an arbitrary-precision type.  */
        seq_t t1 = (seq_t)34 * (seq_t)k * (seq_t)(2 * i - 3) * a_nm1;
        seq_t t2 = (seq_t)3  * (seq_t)(3 * i - 7) * (seq_t)(3 * i - 5) * a_nm2;
        seq_t den = (seq_t)8 * (seq_t)k * (seq_t)i;
        a_n = (t1 + t2) / den;
#else
        /* C build – use 128‑bit intermediates to avoid overflow. */
        unsigned __int128 num =
            (unsigned __int128)34 * (unsigned __int128)k * (unsigned __int128)(2 * i - 3) * (unsigned __int128)a_nm1
          + (unsigned __int128)3  * (unsigned __int128)(3 * i - 7) * (unsigned __int128)(3 * i - 5) * (unsigned __int128)a_nm2;
        unsigned __int128 den =
            (unsigned __int128)8 * (unsigned __int128)k * (unsigned __int128)i;
        a_n = (seq_t)(num / den);
#endif
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
