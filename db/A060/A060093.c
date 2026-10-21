/*
 * A060093  Number of 5-block ordered bicoverings of an unlabeled n-set.
 * https://oeis.org/A060093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9) - 5*binomial(n+5,5) - 10*binomial(n+3,3)
 *            * + 20*binomial(n+2,2) + 30*binomial(n+1,1) - 60*binomial(n,0) *
 *            + 24*binomial(n-1,-1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 43, u64 n <= 568, u128 n <= 1000,
 *            big any n
 * Helpers:   binom_int128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom_int128.h"

seq_t a(idx_t n)
{
    __int128 sum = 0;
    sum += binom_int128(n + 9, 9);
    sum -= (__int128)5 * binom_int128(n + 5, 5);
    sum -= (__int128)10 * binom_int128(n + 3, 3);
    sum += (__int128)20 * binom_int128(n + 2, 2);
    sum += (__int128)30 * binom_int128(n + 1, 1);
    sum -= (__int128)60 * binom_int128(n, 0);
    if (n == 0) sum += (__int128)24;
    return (seq_t)sum;
}
