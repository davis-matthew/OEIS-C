/*
 * A099977  Bisection of Bell numbers, A000110.
 * https://oeis.org/A099977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(2n+1) (Bisection of Bell numbers, A099977)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the odd-indexed Bell numbers.
   Bell numbers are defined by B(0)=1 and
   B(n+1) = Σ_{k=0}^n binom(n, k) * B(k)   for n≥0.  */
#include <stdlib.h>

/* Global cache for Bell numbers */
static idx_t bell_len = 0;    /* number of entries already computed (size of bell array) */
static seq_t *bell = NULL;   /* array of Bell numbers */

/* Ensure that Bell numbers up to index up (inclusive) are computed. */
static void ensure_bell_up_to(idx_t up)
{
    if (up < bell_len) return;   /* already computed */

    /* Allocate or grow the array */
    idx_t new_len = up + 1;
    bell = (seq_t *)realloc(bell, new_len * sizeof(seq_t));
    if (bell == NULL) { /* memory exhaustion – terminate */
        abort();
    }

    /* Initialise new elements to zero */
    for (idx_t i = bell_len; i < new_len; ++i) bell[i] = 0;

    /* Base case */
    if (bell_len == 0) { bell[0] = 1; bell_len = 1; }

    /* Compute missing Bell numbers */
    for (idx_t i = bell_len; i <= up; ++i) {
        if (i == 0) continue;  /* already set */

        seq_t sum = 0;
        seq_t binom = 1;           /* binomial(i-1, 0) */
        idx_t n_minus1 = i - 1;
        for (idx_t k = 0; k < i; ++k) {
            sum += binom * bell[k];
            if (k + 1 < i) {
                /* binom(n-1, k+1) = binom(n-1, k) * (n-1-k) / (k+1) */
                binom = (binom * (seq_t)(n_minus1 - k)) / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;
    }
    bell_len = new_len;
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    /* We need Bell at index m = 2*n + 1. */
    idx_t m = 2 * n + 1;
    ensure_bell_up_to(m);
    return bell[m];
}
