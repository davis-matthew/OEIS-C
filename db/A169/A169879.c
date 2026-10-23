/*
 * A169879  Maximum number of rational points on a smooth absolutely
 *          irreducible projective curve of genus 3 over the field F_3^n.
 * https://oeis.org/A169879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3^n + 1 + 6 * floor(sqrt(3^n)); For n = 3 the actual
 *            maximum is 56, so a(3) = 56
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute q = 3^n */
    seq_t base = (seq_t)3;
    seq_t q = big_pow(base, n);

    /* s = floor(sqrt(q)) */
    seq_t s = floor_sqrt(q);

    /* a = q + 1 + 6 * s */
    seq_t a_val = q + (seq_t)1;
    a_val += s * (seq_t)6;

    /* special adjustment for n = 3 */
    if (n == (idx_t)3)
        a_val -= (seq_t)2;

    return a_val;
}
