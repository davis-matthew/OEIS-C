/*
 * A057966  Triangle T(n,k) of number of minimal 5-covers of a labeled n-set
 *          that cover k points of that set uniquely (k=5,..,n).
 * https://oeis.org/A057966
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Triangle T(n,k) = C(n,k) * S(k,5) * 26^(n-k) The sequence a(n)
 *            enumerates the triangle row‑wise for n ≥ 5
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* gives seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* gives seq_t big_pow(seq_t base, seq_t exp); */

static seq_t *S5 = NULL;        /* Stirling numbers S(k,5)  */
static idx_t S5_size = 0;       /* allocated size of S5 */

/* ------------------------------------------------------------------ */
/*  Ensure that S5[k] for 0 ≤ k ≤ n is computed.                      */
static void ensure_S5(idx_t n)
{
    if (n < 5) return;           /* nothing needed for k < 5 */

    if (S5_size > n) return;     /* already computed */

    seq_t *s = (seq_t *)realloc(S5, (n + 1) * sizeof(seq_t));
    if (!s) exit(EXIT_FAILURE);   /* memory exhaustion */
    S5 = s;

    seq_t prev[6] = {0}, cur[6] = {0};
    prev[0] = 1;   /* S(0,0) = 1 */

    /* fill entries up to n */
    for (idx_t k = 1; k <= n; ++k) {
        cur[0] = 0;
        for (int m = 1; m <= 5; ++m)
            cur[m] = prev[m] * (seq_t)m + prev[m - 1];

        for (int m = 0; m <= 5; ++m) prev[m] = cur[m];

        S5[k] = (k >= 5) ? prev[5] : 0;
    }

    /* zero entries <5 (they stay zero) */
    for (idx_t k = 0; k < 5; ++k) S5[k] = 0;

    S5_size = n + 1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* undefined for n < 5 */

    ensure_S5(n);                 /* make sure Stirling numbers exist */

    /* ----- find the triangle position (row, column) ---------------- */
    idx_t t = n - 5;               /* zero‑based offset in the sequence */

    /* binary search for the row */
    idx_t low = 5, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t total = (mid - 4) * (mid - 3) / 2;   /* terms up to row mid */
        if (total > t)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;

    idx_t prev_total = (row - 5) * (row - 4) / 2;   /* terms before this row */
    idx_t offset   = t - prev_total;
    idx_t k        = 5 + offset;                    /* column index */

    /* ----- compute the entry --------------------------------------- */
    seq_t bin   = binomial(row, k);                 /* C(row,k)   */
    seq_t pow26 = big_pow((seq_t)26, (seq_t)(row - k));  /* 26^(row-k) */
    seq_t term  = bin * pow26;                      /* first product */
    term = term * S5[k];                            /* multiply by S(k,5) */

    return term;
}
