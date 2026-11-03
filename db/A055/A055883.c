/*
 * A055883  Exponential transform of Pascal's triangle A007318.
 * https://oeis.org/A055883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A055883 – Exponential transform of Pascal's triangle
 *  a(n) = B_r · C(r, c)  where r and c are obtained from n:
 *      r = largest integer with r(r+1)/2 ≤ n
 *      c = n – r(r+1)/2
 *  B_r is the r‑th Bell number.
 *
 *  The implementation computes Bell numbers on-demand using the
 *  recurrence
 *      B_0 = 1
 *      B_{m+1} = Σ_{k=0}^{m} C(m, k) · B_k
 *
 *  Only a few hundred rows are required for n ≤ 10 000, so the
 *  approach is fast and uses O(n) time and memory (n = row index).
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* for uint64_t if needed */
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Dynamically grow the Bell number array */
static seq_t *Bell   = NULL;  /* storage for Bell[0..] */
static idx_t bell_cap = 0;    /* allocated size */
static idx_t bell_len = 0;    /* number of entries computed and valid */

/* Ensure that Bell[0..up_to] are computed */
static void ensure_bell(idx_t up_to)
{
    if (bell_len > 0 && up_to < bell_len) return;

    /* Expand storage if necessary */
    if (bell_cap == 0 || up_to + 1 > bell_cap) {
        idx_t new_cap = (bell_cap == 0) ? 16 : bell_cap * 2;
        while (new_cap <= up_to) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, Bell, new_cap);
        Bell = tmp;
        bell_cap = new_cap;
    }

    /* Initialize first value if it hasn't been set */
    if (bell_len == 0) {
        Bell[0] = 1;
        bell_len = 1;
    }

    /* Compute missing Bell numbers */
    for (idx_t m = bell_len; m <= up_to; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;          /* C(m-1,0) = 1   */
        for (idx_t k = 0; k < m; ++k) {
            sum += binom * Bell[k];
            /* advance binomial coefficient */
            binom = (binom * (m - 1 - k)) / (k + 1);
        }
        Bell[m] = sum;
    }
    bell_len = up_to + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r and column c such that
       r(r+1)/2 ≤ n < (r+1)(r+2)/2  and  c = n - r(r+1)/2   */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n) ++r;
    --r;
    idx_t c = n - (r * (r + 1) / 2);

    /* Ensure we have Bell[r] */
    ensure_bell(r);

    /* Compute result */
    seq_t bin = binomial(r, c);
    return Bell[r] * bin;
}
