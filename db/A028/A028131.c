/*
 * A028131  Expansion of 1/((1-4x)(1-6x)(1-7x)(1-9x)).
 * https://oeis.org/A028131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^(n+3)-5*7^(n+3)+5*6^(n+3)-4^(n+3))/30
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    idx_t exp = n + 3;
    seq_t p4 = seq_pow((seq_t)4, exp);
    seq_t p6 = seq_pow((seq_t)6, exp);
    seq_t p7 = seq_pow((seq_t)7, exp);
    seq_t p9 = seq_pow((seq_t)9, exp);

    seq_t term1 = seq_mul((seq_t)5, p6);
    term1 = term1 + p9;

    seq_t term2 = seq_mul((seq_t)5, p7);
    term2 = term2 + p4;

    seq_t diff = term1 - term2;
    return diff / (seq_t)30;
}
