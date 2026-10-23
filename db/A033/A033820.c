/*
 * A033820  Triangle read by rows: T(k,j) =
 *          ((2*j+1)/(k+1))*binomial(2*j,j)*binomial(2*k-2*j,k-j).
 * https://oeis.org/A033820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binom_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A033820 – Triangle read by rows.
 * T(k,j) = ((2*j+1)/(k+1)) * binomial(2*j,j) * binomial(2*k-2*j,k-j)
 * a(n) is the n‑th element of the triangle when scanned row by row,
 * 0‑based.
 */

#include <stdint.h>   /* for uint64_t if needed; seq_t/idx_t are predeclared */
#include "../utils/binom_i.h"

/* Triangle entry T(k,j).  */
static inline seq_t
triangle_term(idx_t k, idx_t j)
{
    seq_t bin1 = binom_i((idx_t)(2 * j), j);
    seq_t bin2 = binom_i((idx_t)(2 * k - 2 * j), k - j);
    seq_t num   = bin1 * bin2;
    num = num * (seq_t)(2 * j + 1);
    return num / (seq_t)(k + 1);
}

/* Public function required by the build system.  */
seq_t a(idx_t n)
{
    /* Find the row k that contains index n.  The cumulative
     * count after row k is (k+1)*(k+2)/2.  */
    idx_t k = 0;
    while (((k + 1) * (k + 2)) / 2 <= n)
        ++k;

    /* Position j within that row.  */
    idx_t prev = (k * (k + 1)) / 2;
    idx_t j    = n - prev;
    return triangle_term(k, j);
}
