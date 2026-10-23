/*
 * A162466  a(n) = 12*a(n-2) for n > 2; a(1) = 1, a(2) = 8.
 * https://oeis.org/A162466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-2) for n>2; a(1)=1, a(2)=8
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=8, a(n)=12*a(n-2) for n>2 */

#include <stdlib.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    /* For n > 2 the recurrence splits into odd and even indices. */
    if (n & 1ULL) {                /* odd n : a(2k-1) = 12^(k-1) */
        idx_t exp = (n - 1) / 2;   /* exponent = (n-1)/2 */
        return big_pow((seq_t)12, (seq_t)exp);
    } else {                       /* even n : a(2k) = 8 * 12^(k-1) */
        idx_t exp = (n - 2) / 2;   /* exponent = (n-2)/2 */
        return (seq_t)8 * big_pow((seq_t)12, (seq_t)exp);
    }
}
