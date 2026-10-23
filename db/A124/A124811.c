/*
 * A124811  Number of 4-ary Lyndon words of length n with exactly three 1s.
 * https://oeis.org/A124811
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-4)*C(n-1,2) if n mod 3 ≠ 0 a(n) = 3^(n-4)*C(n-1,2) –
 *            C(n/3-1,(n-3)/3)*3^((n-3)/3-1) if n mod 3 = 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4)  /* values below the offset are unused */
        return 0;

    enum { seq_t_is_small = (sizeof(seq_t) <= 8) };

    if (seq_t_is_small) {          /* 64‑bit (or smaller) build – use __int128 for safety */
        if (n % 3 != 0) {
            __int128 bin  = (__int128)(n - 1) * (n - 2) / 2;
            __int128 powv = 1;
            for (idx_t i = 0; i < n - 4; ++i) powv *= 3;
            return (seq_t)(bin * powv);
        } else {
            __int128 bin1 = (__int128)(n - 1) * (n - 2) / 2;
            __int128 part1 = bin1;
            for (idx_t i = 0; i < n - 4; ++i) part1 *= 3;

            idx_t exp2 = (n - 3) / 3 - 1;   /* exponent for the second 3 power */
            __int128 part2 = 1;
            for (idx_t i = 0; i < exp2; ++i) part2 *= 3;

            return (seq_t)(part1 - part2);
        }
    } else {                         /* arbitrary‑precision build – use seq_t arithmetic */
        if (n % 3 != 0) {
            seq_t bin  = binomial(n - 1, 2);
            seq_t powv = pow3(n - 4);
            return bin * powv;
        } else {
            seq_t bin1  = binomial(n - 1, 2);
            seq_t part1 = bin1 * pow3(n - 4);
            /* binomial(n/3-1,(n-3)/3) == 1, so omitted */
            seq_t part2 = pow3((n - 3) / 3 - 1);
            return part1 - part2;
        }
    }
}
