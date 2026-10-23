/*
 * A189758  n+[nr/t]+[ns/t]; r=1, s=sin(1), t=cos(1).
 * https://oeis.org/A189758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sec(1)) + floor(n*tan(1))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses rational approximations which are accurate up to n ≈ 10 000. */
#include <stdint.h>

/* Rational approximations:
     sec(1) ≈ 185081571768093 / 100000000000000
     tan(1) ≈ 155740772465490 / 100000000000000
*/
static const seq_t p_sec = 185081571768093ULL;
static const seq_t q_sec = 100000000000000ULL;
static const seq_t p_tan = 155740772465490ULL;
static const seq_t q_tan = 100000000000000ULL;

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, guard against n == 0 */

    seq_t sn = (seq_t)n;
    seq_t sec_floor = (sn * p_sec) / q_sec;
    seq_t tan_floor = (sn * p_tan) / q_tan;
    return sn + sec_floor + tan_floor;
}
