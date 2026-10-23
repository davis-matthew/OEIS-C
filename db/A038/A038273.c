/*
 * A038273  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*7^j.
 * https://oeis.org/A038273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 7^i, where i = max such that i(i+1)/2 ≤
 *            n, j = n - i(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    // Find row i (0‑based) containing index n
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) ++i;

    // Column j within that row
    idx_t j = n - i * (i + 1) / 2;

    seq_t bin = binomial(i, j);          // binomial(i, j)
    seq_t pow = seq_pow((seq_t)7, i);    // 7^i

    return bin * pow;                    // product
}
