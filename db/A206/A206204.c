/*
 * A206204  Number of (n+1)X7 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 17, u64 n <= 46, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206204:
   a(n) = 10 * a(n-3) for n > 16.
   The first 16 terms are provided explicitly:
   a(1)=117084, a(2)=67968, a(3)=60800, a(4)=109720,
   a(5)=236488, a(6)=484704, a(7)=1080648, a(8)=2383520,
   a(9)=5023520, a(10)=11272288, a(11)=24906080, a(12)=51970688,
   a(13)=113273344, a(14)=249152672, a(15)=519714752, a(16)=1132733728.
   For n > 16 the value is 10^t * a(base) where base depends on n % 3:
   n%3==1 → base=16, n%3==2 → base=14, n%3==0 → base=15.
   The exponent t = (n - base)/3.
*/

#include <stdint.h>

/* Fast exponentiation of 10^exp using the seq_t type */
static inline seq_t pow10_seq(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 10;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Initial 16 terms */
    static const seq_t init[16] = {
        117084ULL, 67968ULL, 60800ULL, 109720ULL,
        236488ULL, 484704ULL, 1080648ULL, 2383520ULL,
        5023520ULL, 11272288ULL, 24906080ULL, 51970688ULL,
        113273344ULL, 249152672ULL, 519714752ULL, 1132733728ULL
    };

    if (n <= 16) {
        return init[n - 1];
    }

    idx_t base;
    switch (n % 3U) {
        case 1U: base = 16; break;
        case 2U: base = 14; break;
        default: /* n % 3 == 0 */
            base = 15; break;
    }

    idx_t t = (n - base) / 3;
    seq_t value = init[base - 1];
    value *= pow10_seq(t);
    return value;
}
