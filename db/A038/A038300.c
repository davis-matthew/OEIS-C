/*
 * A038300  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*10^j.
 * https://oeis.org/A038300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*9^(i-j)*10^j, where i =
 *            floor((sqrt(8n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* find the row i that contains the n‑th entry (0‑based)   */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    /* position j within that row                               */
    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;

    /* compute the entry: binomial(i,j) * 9^(i-j) * 10^j */
    seq_t bin   = binomial(i, j);
    seq_t pow9  = seq_pow((seq_t)9, i - j);
    seq_t pow10 = seq_pow((seq_t)10, j);

    return bin * pow9 * pow10;
}
