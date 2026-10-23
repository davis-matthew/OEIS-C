/*
 * A024584  a(n) = floor(1/frac(n*Pi)).
 * https://oeis.org/A024584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/frac(n*Pi))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 30 decimal digits of Pi: 3.141592653589793238462643383279 */
    /* 10^30 as 128‑bit integer */
    static unsigned __int128 PI_NUM = 0;
    static unsigned __int128 PI_DEN = 0;

    if (PI_DEN == 0) {
        /* construct PI_NUM from decimal string */
        const char *s = "3141592653589793238462643383279";
        unsigned __int128 val = 0;
        for (; *s; ++s)
            val = val * 10 + (*s - '0');
        PI_NUM = val;
        /* 10^30 */
        unsigned __int128 d = 1;
        for (int i = 0; i < 30; ++i) d *= 10;
        PI_DEN = d;
    }

    unsigned __int128 prod = (unsigned __int128)n * PI_NUM;
    unsigned __int128 rem = prod % PI_DEN;
    /* rem should never be zero for n >= 1 */
    unsigned __int128 res = PI_DEN / rem;
    return (seq_t)res;
}
