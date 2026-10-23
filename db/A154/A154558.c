/*
 * A154558  Triangle read by rows: binomial(n-1,k-1)*A001006(k).
 * https://oeis.org/A154558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r-1,k-1) * Motzkin(k), where r and k are the row
 *            and column indices of n in the triangle. The triangle rows are:
 *            1; 1, 2; 1, 4, 4; ... The Motzkin numbers are M(0)=1, M(1)=1,
 *            M(2)=2, M(3)=4, M(4)=9, ... They are stored in a static cache
 *            for speed
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* Cache for Motzkin numbers.  The maximum k needed for n ≤ 10000 is about 140;
   a generous limit of 200 is sufficient.  The array is static to avoid dynamic
   allocation that may fail when seq_t is an arbitrary‑precision type. */
static seq_t motzkin[200];
static idx_t motzkin_len = 0;

/* Return M(k) (Motzkin number, k ≥ 0).  The cache is expanded lazily. */
static seq_t get_motzkin(idx_t k)
{
    /* Initialise cache for M(0) and M(1) if empty. */
    if (motzkin_len == 0) {
        motzkin[0] = (seq_t)1;     /* M(0) = 1 */
        motzkin[1] = (seq_t)1;     /* M(1) = 1 */
        motzkin_len = 2;
    }

    /* Expand cache up to index k, if necessary. */
    if (k >= motzkin_len) {
        /* Expand until we have at least k+1 entries.  The extra space above
           the current limit (200) is enough for all requested indices.  */
        if (k >= 200) {
            /* Should never happen for the intended usage (n ≤ 10000).  */
            size_t new_cap = (size_t)k + 1;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            /* Copy already computed values. */
            for (size_t i = 0; i < motzkin_len; ++i)
                new_cache[i] = motzkin[i];
            /* The old static array is not freed; we only use the new one. */
            for (size_t i = motzkin_len; i <= k; ++i) {
                if (i < 2) {
                    new_cache[i] = (seq_t)1;
                } else {
                    seq_t num1 = ((seq_t)(2 * i + 1)) * new_cache[i - 1];
                    seq_t num2 = ((seq_t)(3 * i - 3)) * new_cache[i - 2];
                    new_cache[i] = (num1 + num2) / (i + 2);
                }
            }
            motzkin_len = k + 1;
            return new_cache[k];
        } else {
            for (idx_t i = motzkin_len; i <= k; ++i) {
                if (i < 2) {
                    motzkin[i] = (seq_t)1;
                } else {
                    seq_t num1 = ((seq_t)(2 * i + 1)) * motzkin[i - 1];
                    seq_t num2 = ((seq_t)(3 * i - 3)) * motzkin[i - 2];
                    motzkin[i] = (num1 + num2) / (i + 2);
                }
            }
            motzkin_len = k + 1;
        }
    }

    return motzkin[k];
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0, harmless */

    /* Find row r such that r*(r+1)/2 ≥ n by incremental search. */
    idx_t r = 1;
    idx_t tri = 1;          /* triangular number r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* Column k within this row (1‑based). */
    idx_t prev_tri = tri - r;   /* (r-1)*r/2 */
    idx_t k = n - prev_tri;     /* 1 ≤ k ≤ r */

    /* Compute the triangular entry: binomial(r-1,k-1) * Motzkin(k). */
    seq_t bin = binom(r - 1, k - 1);
    seq_t mot = get_motzkin(k);
    return bin * mot;
}
