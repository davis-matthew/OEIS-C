/*
 * A165167  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) … a(7) are given. For n ≥ 9 a(n) = 33 a(n-1) + 33 a(n-2) +
 *            33 a(n-3) + 33 a(n-4) + 33 a(n-5) + 33 a(n-6) + 33 a(n-7) – 561
 *            a(n-8). For n = 8 the right‐hand side receives an additional
 *            constant +1 coming from the numerator of the generating function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first eight terms (offset 0) */
    static const seq_t first[8] = {
        (seq_t)1ULL,        /* a(0) */
        (seq_t)35ULL,       /* a(1) */
        (seq_t)1190ULL,     /* a(2) */
        (seq_t)40460ULL,    /* a(3) */
        (seq_t)1375640ULL,  /* a(4) */
        (seq_t)46771760ULL, /* a(5) */
        (seq_t)1590239840ULL, /* a(6) */
        (seq_t)54068154560ULL /* a(7) */
    };

    if (n < 8)
        return first[n];

    /* sliding window holding the last 8 values:
       window[0] = a(i-8), window[7] = a(i-1)  */
    seq_t window[8] = {
        first[0], first[1], first[2], first[3],
        first[4], first[5], first[6], first[7]
    };

    for (idx_t i = 8; i <= n; ++i)
    {
        unsigned __int128 sum128 = 0;
        for (int k = 1; k < 8; ++k)
            sum128 += (unsigned __int128)window[k];

        unsigned __int128 val128 = 33 * sum128
                                  - 561 * (unsigned __int128)window[0]
                                  + (i == 8 ? (unsigned __int128)1 : (unsigned __int128)0);
        seq_t val = (seq_t)val128;

        /* shift window left and insert the new value */
        for (int k = 0; k < 7; ++k)
            window[k] = window[k + 1];
        window[7] = val;
    }

    return window[7];
}
