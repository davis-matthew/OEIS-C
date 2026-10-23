/*
 * A038254  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*12^j.
 * https://oeis.org/A038254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*5^(i-j)*12^j * where i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"   /* provides big_pow(seq_t base, seq_t exp)  */
#include "../utils/binom.h"    /* provides binom(idx_t a, idx_t b)       */

/* integer square root of an unsigned 64‑bit number: floor(sqrt(x)) */
static inline idx_t int_sqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) <= x / (r + 1))
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* compute the row (i) and column (j) of the n‑th entry */
    idx_t d = int_sqrt_u64(8 * n + 1);
    idx_t i = (d - 1) / 2;
    idx_t j = n - i * (i + 1) / 2;

    /* compute the three factors of the term */
    seq_t pow5  = big_pow((seq_t)5, (seq_t)(i - j)); /* 5^(i-j) */
    seq_t pow12 = big_pow((seq_t)12, (seq_t)j);     /* 12^j    */
    seq_t bin   = binom(i, j);                     /* binomial(i,j) */

    /* combine the factors */
    return bin * pow5 * pow12;
}
