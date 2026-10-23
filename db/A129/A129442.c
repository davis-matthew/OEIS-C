/*
 * A129442  Expansion of c(x)*c(x*c(x)) where c(x) is the g.f. of A000108.
 * https://oeis.org/A129442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 3*n*(n+1)*a(n) = 14*n*(2*n-1)*a(n-1) - 4*(4*n-5)*(4*n-3)*a(n-2)
   with a(0)=1, a(1)=2.  */
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t result;

#if defined(__cplusplus)
    /* C++ path: choose implementation based on whether seq_t is an
       integral type (e.g., uint64_t) or a big‑integer class. */
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 p2 = 1, p1 = 2, cur = 0;
        for (idx_t i = 2; i <= n; ++i) {
            unsigned __int128 t1 = (unsigned __int128)14 * i * (2 * i - 1) * p1;
            unsigned __int128 t2 = (unsigned __int128)4 * (4 * i - 5) * (4 * i - 3) * p2;
            unsigned __int128 numer = t1 - t2;
            unsigned __int128 denom = (unsigned __int128)3 * i * (i + 1);
            cur = numer / denom;
            p2 = p1;
            p1 = cur;
        }
        result = (seq_t)cur;
    } else {
        /* Big‑integer implementation: use normal arithmetic operators */
        seq_t a0 = (seq_t)1;
        seq_t a1 = (seq_t)2;
        seq_t a2;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t f1 = (seq_t)(14ull * (unsigned long long)i * (2 * i - 1));
            seq_t f2 = (seq_t)(4ull * (unsigned long long)(4 * i - 5) *
                               (unsigned long long)(4 * i - 3));
            seq_t t1 = a1 * f1;
            seq_t t2 = a0 * f2;
            seq_t numer = t1 - t2;
            seq_t denom = (seq_t)(3ull * (unsigned long long)i * (i + 1));
            a2 = numer / denom;
            a0 = a1;
            a1 = a2;
        }
        result = a2;
    }
#else
    /* C path: use 128‑bit integers to avoid overflow (uint64_t seq_t). */
    unsigned __int128 p2 = 1, p1 = 2, cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 t1 = (unsigned __int128)14 * i *
                              (2 * i - 1) * p1;
        unsigned __int128 t2 = (unsigned __int128)4 * (4 * i - 5) *
                              (4 * i - 3) * p2;
        unsigned __int128 numer = t1 - t2;
        unsigned __int128 denom = (unsigned __int128)3 * i * (i + 1);
        cur = numer / denom;
        p2 = p1;
        p1 = cur;
    }
    result = (seq_t)cur;
#endif

    return result;
}
