/*
 * A168307  The fourth left hand column of triangle A167591.
 * https://oeis.org/A168307
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^n*(27*n^7 - 434*n^6 + 2289*n^5 - 5705*n^4 + 7938*n^3 -
 *            6461*n^2 + 2346*n)/10080
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 36, i128 n <= 89,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n >= 4 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Small‑integer (≤64‑bit) implementation using 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __int128 n128 = n;
        __int128 n2   = n128 * n128;
        __int128 n3   = n2   * n128;
        __int128 n4   = n3   * n128;
        __int128 n5   = n4   * n128;
        __int128 n6   = n5   * n128;
        __int128 n7   = n6   * n128;

        const __int128 c27   = 27LL,  c434  = 434LL,  c2289 = 2289LL;
        const __int128 c5705 = 5705LL, c7938 = 7938LL,  c6461 = 6461LL;
        const __int128 c2346 = 2346LL;

        __int128 poly = c27   * n7
                      - c434  * n6
                      + c2289 * n5
                      - c5705 * n4
                      + c7938 * n3
                      - c6461 * n2
                      + c2346 * n128;

        __int128 pow  = (__int128)1 << n;           /* 2^n */
        __int128 res  = (poly * pow) / (__int128)10080;
        return (seq_t)res;
    } else {
        /* Arbitrary‑precision implementation. */
        seq_t n_s  = (seq_t)n;
        seq_t n2   = n_s * n_s;
        seq_t n3   = n2   * n_s;
        seq_t n4   = n3   * n_s;
        seq_t n5   = n4   * n_s;
        seq_t n6   = n5   * n_s;
        seq_t n7   = n6   * n_s;

        const seq_t c27   = (seq_t)27,   c434  = (seq_t)434,   c2289 = (seq_t)2289;
        const seq_t c5705 = (seq_t)5705, c7938 = (seq_t)7938,  c6461 = (seq_t)6461;
        const seq_t c2346 = (seq_t)2346;

        seq_t poly = c27   * n7
                   - c434  * n6
                   + c2289 * n5
                   - c5705 * n4
                   + c7938 * n3
                   - c6461 * n2
                   + c2346 * n_s;

        seq_t pow  = pow2(n);                         /* 2^n */
        seq_t res  = (poly * pow) / (seq_t)10080;
        return res;
    }
}
