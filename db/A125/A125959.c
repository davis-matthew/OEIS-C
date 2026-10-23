/*
 * A125959  Infinite array of nine columns, read by rows: A(j,k) = digital
 *          root of j*k for j >= 1, 1 <= k <= 9.
 * https://oeis.org/A125959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, easy
 * Formula:   a(n) = digital_root((1+floor((n-1)/9))*(1+((n-1)%9)))
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute row (j) and column (k) of the 9×9 digital‑root table */
    idx_t m   = n - 1;            /* zero‑based index */
    idx_t j   = m / 9 + 1;        /* row number, 1‑based */
    idx_t k   = m % 9 + 1;        /* column number, 1‑based */

    /* Only the product modulo 9 matters for the digital root. */
    idx_t j_mod9 = j % 9;         /* 0..8 */
    idx_t k_mod9 = k % 9;         /* 0..8 */
    idx_t prod_mod9 = (j_mod9 * k_mod9) % 9;  /* 0..8 */

    /* Digital root is 9 when the product is divisible by 9, otherwise the
       remainder itself. */
    seq_t result = prod_mod9 ? (seq_t)prod_mod9 : (seq_t)9;
    return result;
}
