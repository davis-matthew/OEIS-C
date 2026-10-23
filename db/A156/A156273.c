/*
 * A156273  a(n) = 9^n*Catalan(n).
 * https://oeis.org/A156273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n*A000108(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);              /* Catalan(n) */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n); /* 9^n */
    return pow9 * cat;                    /* 9^n * Catalan(n) */
}
