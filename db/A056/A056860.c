/*
 * A056860  Triangle T(n,k) = number of element-subset partitions of {1..n}
 *          with n-k+1 equalities (n >= 1, 1 <= k <= n).
 * https://oeis.org/A056860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the concatenation of the triangle T, where T(r,k) =
 *            binomial(r‑1,k‑1) * B(k‑1) (B = Bell numbers). For index n, let
 *            r be the unique integer with r(r+1)/2 ≥ n; let k = n – r(r‑1)/2
 *            (1‑based column). Then a(n) = T(r,k)
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n ≥ 1 in the test harness, but guard anyway */
        return 0;

    /* 1. Determine row r and column k from the triangular indexing. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t k = n - r * (r - 1) / 2;   /* 1 ≤ k ≤ r */

    /* 2. Make sure Bell numbers up to B(k-1) are available. */
    static seq_t bell[200];          /* enough for the problem limits (k ≤ ~140) */
    static int bell_sz = 0;          /* next index to compute (size of valid part) */

    if (bell_sz == 0) {             /* initialise */
        bell[0] = 1;
        bell_sz = 1;
    }

    /* Compute any missing Bell numbers up to index k-1 */
    while (bell_sz <= (int)(k - 1)) {
        int n_bell = bell_sz;        /* we want B(n_bell) */
        seq_t sum = 0;
        for (int j = 0; j < n_bell; ++j) {
            sum += binomial(n_bell - 1, j) * bell[j];
        }
        bell[bell_sz] = sum;
        ++bell_sz;
    }

    /* 3. Compute the triangle entry T(r,k) = C(r-1, k-1) * B(k-1). */
    seq_t choose = binomial(r - 1, k - 1);
    return choose * bell[(int)(k - 1)];
}
