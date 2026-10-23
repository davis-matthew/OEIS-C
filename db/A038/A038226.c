/*
 * A038226  Triangle read by rows: (i,j)-th entry is
 *          binomial(i,j)*3^(i-j)*8^j.
 * https://oeis.org/A038226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the row‑wise flattening of the triangle with (i,j)-th
 *            entry = binomial(i,j) * 3^(i‑j) * 8^j. For a given n, let i be
 *            the unique integer with i*(i+1)/2 ≤ n < (i+1)*(i+2)/2, and let j
 *            = n – i*(i+1)/2. Then a(n) = binomial(i,j)*3^(i-j)*8^j
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 60, u64 n <= 220, u128 n <= 766,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    idx_t i = 0;
    /* find the row i that contains n */
    while (1) {
        idx_t next = (i + 1) * (i + 2) / 2;
        if (next > n) break;
        i++;
    }

    idx_t tri = i * (i + 1) / 2;
    idx_t j = n - tri;          /* column index within the row */

    seq_t val = binomial(i, j); /* binomial coefficient */

    /* powers of 3 and 8.  The exponent fits in int for all supported n. */
    val *= pow_seq((seq_t)3, (int)(i - j));
    val *= pow_seq((seq_t)8, (int)j);

    return val;
}
