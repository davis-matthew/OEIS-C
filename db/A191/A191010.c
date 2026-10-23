/*
 * A191010  a(n) = (n*4^(n+1) + (6*4^(n+1)+(-1)^n)/5)/5.
 * https://oeis.org/A191010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (4^(n+1)*(5*n+6)+(-1)^n)/25
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence equivalent: a(n)=7*a(n-1)-8*a(n-2)-16*a(n-3) for n>2 */
/* Implementation uses the closed form, avoiding intermediate overflow
 * by performing division by 25 early. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 4^(n+1) */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)(n + 1));

    /* decompose pow4 as 25*k + r */
    seq_t k = pow4 / (seq_t)25;
    seq_t r = pow4 % (seq_t)25;

    /* q = 5*n + 6 */
    seq_t q = (seq_t)5 * (seq_t)n + (seq_t)6;

    seq_t part1 = k * q;

    /* The second part is small (≤149) */
    seq_t part2;
    if ((n & (idx_t)1) == 0) /* n even: (-1)^n = +1 */
        part2 = (r * q + (seq_t)1) / (seq_t)25;
    else /* n odd: (-1)^n = -1 */
        part2 = (r * q - (seq_t)1) / (seq_t)25;

    return part1 + part2;
}
