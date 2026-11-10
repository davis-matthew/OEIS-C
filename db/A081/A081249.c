/*
 * A081249  Partial sums of A081134.
 * https://oeis.org/A081249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum{j=1..n} A081134(j), where A081134(j) = min(j-3^k,
 *            3^(k+1)-j), k = floor(log_3(j))
 * Range:     u8 n <= 44, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t power = 1;
    idx_t j;
    for (j = 1; j <= n; j++) {
        seq_t jj = (seq_t)j;
        while (power * (seq_t)3 <= jj && power * (seq_t)3 > power)
            power = power * (seq_t)3;
        seq_t down = jj - power;
        seq_t up = power * (seq_t)3 - jj;
        sum += (down < up) ? down : up;
    }
    return sum;
}
