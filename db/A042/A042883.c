/*
 * A042883  Denominators of continued fraction convergents to sqrt(973).
 * https://oeis.org/A042883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1806446 * a(n-8) - a(n-16) for n > 15 Initial values:
 *            a(0)=1, a(1)=5, a(2)=26, a(3)=57, a(4)=482, a(5)=1021,
 *            a(6)=5587, a(7)=28956, a(8)=1800859, a(9)=9033251,
 *            a(10)=46967114, a(11)=102967479, a(12)=870706946,
 *            a(13)=1844381371, a(14)=10092613801, a(15)=52307450376
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[16] = {
    (seq_t)1ULL,       (seq_t)5ULL,       (seq_t)26ULL,      (seq_t)57ULL,
    (seq_t)482ULL,     (seq_t)1021ULL,    (seq_t)5587ULL,    (seq_t)28956ULL,
    (seq_t)1800859ULL, (seq_t)9033251ULL, (seq_t)46967114ULL,(seq_t)102967479ULL,
    (seq_t)870706946ULL,(seq_t)1844381371ULL,(seq_t)10092613801ULL,
    (seq_t)52307450376ULL
};

seq_t a(idx_t n)
{
    if (n < 16)
        return init[(size_t)n];

    seq_t buf[16];
    for (size_t i = 0; i < 16; ++i)
        buf[i] = init[i];

    for (idx_t i = 16; i <= n; ++i)
    {
        seq_t val = (seq_t)1806446 * buf[(i - 8) % 16] - buf[(i - 16) % 16];
        buf[i % 16] = val;
    }

    return buf[n % 16];
}
