/*
 * A189232  Triangle read by rows: Number of crossing set partitions of
 *          {1,2,...,n} into k blocks.
 * https://oeis.org/A189232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle of crossing set
 *            partitions read by rows. For a given linear index n (1‑based)
 *            let r be the row number (the smallest r so that r*(r+1)/2 ≥ n)
 *            and k = n − r*(r−1)/2. Then a(n) = T(r,k) = S₂(r,k) −
 *            C(r,k−1)*C(r,k)/r where S₂ is the Stirling number of the
 *            second...
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, decoupage
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/decoupage.h"

/* Cached array of Stirling numbers S₂(n,k), 1 ≤ k ≤ n.  S[n] points to
   an array of length n+1 with indices 0…n.  Index 0 is unused. */
static seq_t **S = NULL;
static size_t max_n = 0;  /* largest n whose Stirling row is available */

/* Ensure Stirling numbers up to new_n are available. */
static void ensure_stirling(size_t new_n)
{
    if (new_n <= max_n) return;

    /* Expand the array of row pointers. */
    if (S == NULL) {
        S = (seq_t **)malloc((new_n + 1) * sizeof(seq_t *));
    } else {
        S = (seq_t **)realloc(S, (new_n + 1) * sizeof(seq_t *));
    }
    if (!S) abort();

    /* Build new rows. */
    for (size_t n = max_n + 1; n <= new_n; ++n) {
        seq_t *row = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (!row) abort();
        row[0] = 0;
        row[1] = 1;
        /* For 2 ≤ k ≤ n−1 use recurrence. */
        for (size_t k = 2; k < n; ++k) {
            /* S₂(n,k) = k·S₂(n−1,k) + S₂(n−1,k−1) */
            seq_t term1 = (seq_t)k * S[n - 1][k];
            seq_t term2 = S[n - 1][k - 1];
            row[k] = term1 + term2;
        }
        /* The diagonal element S₂(n,n) = 1. */
        row[n] = 1;
        S[n] = row;
    }
    max_n = new_n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    size_t r, k;
    decoupage(n, &r, &k);
    ensure_stirling(r);

    seq_t s2 = S[r][k];
    seq_t bin1 = binomial(r, k - 1);
    seq_t bin2 = binomial(r, k);
    seq_t prod = bin1 * bin2;
    seq_t term = prod / (seq_t)r;   /* integer division, term is integer */

    return s2 - term;
}
