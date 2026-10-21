/*
 * A061164  a(n) = (20*n)!*n!/((10*n)!*(7*n)!*(4*n)!).
 * https://oeis.org/A061164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(20*n,10*n)*binomial(10*n,3*n)/binomial(4*n,n)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The helpers binomial and the arithmetic of seq_t take care
     * of arbitrary‑precision when needed. For 64‑bit builds the
     * division is exact for every n whose result fits into seq_t. */
    seq_t first  = binomial(20 * n, 10 * n);   /* (20n choose 10n) */
    seq_t second = binomial(10 * n, 3 * n);    /* (10n choose 3n) */
    seq_t denom  = binomial(4 * n, n);         /* (4n choose n)  */
    seq_t prod   = first * second;             /* multiply before dividing */
    return prod / denom;                       /* exact division */
}
