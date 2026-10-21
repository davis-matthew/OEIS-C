/*
 * A062810  a(n) = Sum_{i=1..n} i^(n - i) + (n - i)^i.
 * https://oeis.org/A062810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i^(n - i) + (n - i)^i
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Optimised by pairing symmetric terms and adding the last term separately. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* the sum for i=1..floor((n-1)/2) */
    const idx_t mid = (n - 1) / 2;
    seq_t sum = 0;

    for (idx_t i = 1; i <= mid; ++i) {
        seq_t p1 = big_pow((seq_t)i,        (seq_t)(n - i));
        seq_t p2 = big_pow((seq_t)(n - i), (seq_t)i);
        sum += p1;
        sum += p2;
    }

    /* double-count symmetric pairs */
    sum += sum;

    /* middle term when n is even */
    if ((n & 1) == 0) {
        seq_t m = (seq_t)(n / 2);
        seq_t p = big_pow(m, m);
        sum += p;    /* + (n/2)^(n/2) */
        sum += p;    /* + (n/2)^(n/2) again */
    }

    /* final term i = n contributes 1 */
    sum += (seq_t)1;

    return sum;
}
