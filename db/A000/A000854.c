/*
 * A000854  Number of n-input 3-output switching networks under action of
 *          complementing group C(2,n) on inputs and S(3) and C(2,3) on
 *          outputs.
 * https://oeis.org/A000854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/48)*(8^(2^n) + 20*(2^n-1)*8^(2^(n-1)) + 8*2^(2^n) +
 *            16*(2^n-1)*2^(2^(n-1)) + 6*4^(2^n))/(2^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a854_from_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a854_from_u128.h"

seq_t a(idx_t n) {
    if (n <= 5) {
        /* exact native computation; numerator < 2^128 for n <= 5 */
        unsigned __int128 m = (unsigned __int128)1 << n;          /* 2^n */
        unsigned m1 = (unsigned)(1u << (n - 1));                   /* 2^(n-1) */
        unsigned mm = (unsigned)m;
        unsigned __int128 one = 1;
        unsigned __int128 N = (one << (3 * mm))
                            + (unsigned __int128)20 * (m - 1) * (one << (3 * m1))
                            + (unsigned __int128)8 * (one << mm)
                            + (unsigned __int128)16 * (m - 1) * (one << m1)
                            + (unsigned __int128)6 * (one << (2 * mm));
        unsigned __int128 v = N / ((unsigned __int128)48 * m);
        return a854_from_u128(v);
    }
    {
        seq_t zero = 0;
        if (zero - 1 > zero) {
            /* fixed-width unsigned type: value cannot fit for n >= 6 */
            return 0;
        }
    }
    {
        idx_t m = (idx_t)1 << n;
        idx_t m1 = (idx_t)1 << (n - 1);
        seq_t one = 1;
        seq_t pm = one << n;
        seq_t N = (one << (3 * m))
                + (seq_t)20 * (pm - 1) * (one << (3 * m1))
                + (seq_t)8 * (one << m)
                + (seq_t)16 * (pm - 1) * (one << m1)
                + (seq_t)6 * (one << (2 * m));
        seq_t D = (seq_t)48 << n;
        return N / D;
    }
}
