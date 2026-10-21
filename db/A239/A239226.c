/*
 * A239226  a(n) = A000984(n) * A081085(n).
 * https://oeis.org/A239226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000984(n) * A081085(n) * Recurrence: a(n) = (a(n‑1) * 8
 *            * (2n-1) * (3n^2-3n+1) - a(n‑2) * 128 * (n-1) * (2n-1) * (2n-3))
 *            / n^3 * a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifdef __cplusplus
/* C++ implementation – works for 64‑bit and arbitrary‑precision seq_t */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    if constexpr (sizeof(seq_t) <= 8) {
        /* Small integer path: use 128‑bit intermediates to avoid overflow */
        using int128_t = unsigned __int128;
        seq_t a0 = 1, a1 = 8, an = 0;

        for (idx_t i = 2; i <= n; ++i) {
            int128_t coeff1 = (int128_t)8 * (int128_t)(2*i - 1) *
                              (int128_t)(3*i*i - 3*i + 1);
            int128_t coeff2 = (int128_t)128 * (int128_t)(i-1) *
                              (int128_t)(2*i - 1) * (int128_t)(2*i - 3);
            int128_t denom  = (int128_t)i * (int128_t)i * (int128_t)i;

            int128_t term1 = coeff1 * (int128_t)a1;
            int128_t term2 = coeff2 * (int128_t)a0;
            int128_t numer = term1 - term2;

            an = (seq_t)(numer / denom);

            a0 = a1;
            a1 = an;
        }
        return an;
    } else {
        /* Arbitrary‑precision path: all arithmetic in seq_t */
        seq_t a0 = 1, a1 = 8, an = 0;

        for (idx_t i = 2; i <= n; ++i) {
            seq_t coeff1 = (seq_t)8 * (seq_t)(2*i - 1) *
                           (seq_t)(3*i*i - 3*i + 1);
            seq_t coeff2 = (seq_t)128 * (seq_t)(i-1) *
                           (seq_t)(2*i - 1) * (seq_t)(2*i - 3);
            seq_t denom  = (seq_t)i * (seq_t)i * (seq_t)i;

            seq_t numer = coeff1 * a1 - coeff2 * a0;
            an = numer / denom;          /* division is exact */
            a0 = a1;
            a1 = an;
        }
        return an;
    }
}
#else
/* C implementation – uses 128‑bit intermediates for correct values up to 64‑bit seq_t */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a0 = 1, a1 = 8, an = 0;

    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 coeff1 = (unsigned __int128)8 * (unsigned __int128)(2*i - 1) *
                                   (unsigned __int128)(3*i*i - 3*i + 1);
        unsigned __int128 coeff2 = (unsigned __int128)128 * (unsigned __int128)(i-1) *
                                   (unsigned __int128)(2*i - 1) * (unsigned __int128)(2*i - 3);
        unsigned __int128 denom  = (unsigned __int128)i * (unsigned __int128)i * (unsigned __int128)i;

        unsigned __int128 term1 = coeff1 * (unsigned __int128)a1;
        unsigned __int128 term2 = coeff2 * (unsigned __int128)a0;
        unsigned __int128 numer = term1 - term2;

        an = (seq_t)(numer / denom);

        a0 = a1;
        a1 = an;
    }
    return an;
}
#endif
