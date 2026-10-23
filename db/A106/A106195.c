/*
 * A106195  Riordan array (1/(1-2*x), x*(1-x)/(1-2*x)).
 * https://oeis.org/A106195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, c) where * r = max { r | r(r+1)/2 ≤ n } (row index)
 *            * c = n – r(r+1)/2 (column index) * T(r, c) = Σ_{i=0}^{r−c}
 *            C(c,i)·C(r−c,i)·2^{\,r−c−i} * This yields the flattened Riordan
 *            array (1/(1−2x), x(1−x)/(1−2x))
 * Range:     u8 n <= 28, u16 n <= 93, u32 n <= 330, u64 n <= 1233,
 *            u128 n <= 4581, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t
row_of_index(idx_t n)
{
    idx_t r = 0;
    idx_t sum = 0;                /* sum of 1..r = r(r+1)/2   */
    while (sum + (r + 1) <= n) {
        r++;
        sum += r;
    }
    return r;
}

/* Compute T(r, c) by the closed‑form summation. */
static seq_t
T_val(idx_t r, idx_t c)
{
    seq_t sum = 0;
    idx_t max_i = r - c;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t b1 = binom(c, i);          /* C(c,i)  */
        seq_t b2 = binom(r - c, i);      /* C(r−c,i) */
        seq_t term = b1 * b2;
        idx_t exp = max_i - i;           /* r−c−i   */
        seq_t pow2 = 1;
        for (idx_t e = 0; e < exp; ++e)
            pow2 <<= 1;
        term *= pow2;
        sum += term;
    }
    return sum;
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    unsigned __int128 tri128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)tri128;
    return T_val(r, c);
}
