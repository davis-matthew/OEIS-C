/*
 * A211912  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal, vertical, diagonal or antidiagonal neighbor, and
 *          containing the value n(n+1)/2-3.
 * https://oeis.org/A211912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9), for n >
 *            10 Initial values: a(1)=0, a(2)=1, a(3)=10, a(4)=214, a(5)=1946,
 *            a(6)=10431, a(7)=40561, a(8)=127275, a(9)=342434, a(10)=820396
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* first ten values explicit */
    static const seq_t init[10] = {
        (seq_t)0,   (seq_t)1,   (seq_t)10,   (seq_t)214,
        (seq_t)1946, (seq_t)10431, (seq_t)40561, (seq_t)127275,
        (seq_t)342434, (seq_t)820396
    };

    /* offset 1: guard against n==0 */
    if (n == 0) return (seq_t)0;
    if (n <= 10) return init[n-1];

    /* use a sliding window of length 9: a(k-1), …, a(k-9) */
    seq_t last[9];
    for (idx_t i = 0; i < 9; ++i)
        last[i] = init[9 - i];          /* last[0] = a(10), … , last[8] = a(2) */

    for (idx_t cur = 11; cur <= n; ++cur) {
        seq_t value;
#ifdef __cplusplus
        /* C++ build – big‑int objects can multiply directly */
        value = last[0] * 9 + last[2] * 84 + last[4] * 126 + last[6] * 36 + last[8]
                - (last[1] * 36 + last[3] * 126 + last[5] * 84 + last[7] * 9);
#else
#  if defined(__SIZEOF_INT128__)
        /* C build – use 128‑bit intermediates to avoid overflow for uint64_t */
        __int128 pos = (__int128)last[0] * 9
                     + (__int128)last[2] * 84
                     + (__int128)last[4] * 126
                     + (__int128)last[6] * 36
                     + (__int128)last[8];

        __int128 neg = (__int128)last[1] * 36
                     + (__int128)last[3] * 126
                     + (__int128)last[5] * 84
                     + (__int128)last[7] * 9;

        value = (seq_t)(pos - neg);
#  else
        /* fallback if __int128 unavailable – safe for the tested range */
        seq_t pos = 0, neg = 0;
        pos += last[0] * 9;
        pos += last[2] * 84;
        pos += last[4] * 126;
        pos += last[6] * 36;
        pos += last[8];
        neg += last[1] * 36;
        neg += last[3] * 126;
        neg += last[5] * 84;
        neg += last[7] * 9;
        value = pos - neg;
#  endif
#endif

        /* shift the window: drop a(k-9), push new value */
        for (idx_t j = 8; j > 0; --j) last[j] = last[j-1];
        last[0] = value;
    }

    return last[0];   /* a(n) */
}
