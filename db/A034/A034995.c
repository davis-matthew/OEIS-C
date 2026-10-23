/*
 * A034995  Smallest square starting with a string of n 9's.
 * https://oeis.org/A034995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest perfect square whose decimal representation
 *            begins with n consecutive 9's
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    /* 10^n and the prefix m = 10^n – 1, i.e. n 9's. */
    seq_t ten_pow_n = pow10(n);
    seq_t prefix    = ten_pow_n - 1;

    /* Search over the number of trailing digits m.
       We need the smallest square k² such that
         prefix·10^m ≤ k² < 10^n·10^m. */
    for (idx_t m = 0; ; ++m) {
        seq_t ten_pow_m = pow10(m);
        seq_t low  = prefix * ten_pow_m;   /* inclusive lower bound */
        seq_t high = ten_pow_n * ten_pow_m;/* exclusive upper bound */

        /* ceil(sqrt(low)) */
        seq_t k = floor_sqrt(low);
        if (k * k < low) ++k;

        seq_t sq = k * k;
        if (sq < high) return sq;          /* first square in the interval */
    }
}
