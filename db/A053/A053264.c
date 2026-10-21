/*
 * A053264  Coefficients of the '5th-order' mock theta function F_0(q).
 * https://oeis.org/A053264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, if n = 0; a(n) = sum_{m>=1, 2m^2 <= n}
 *            P_{odd<=2m-1}(n-2m^2), where P_{odd<=k}(t) counts partitions of
 *            t using only odd parts <= k. This follows from the generating
 *            function F_0(q) = Σ_{m≥0} q^(2m²)/((1-q)(1-q³)…(1-q^(2m-1)))
 * Range:     u8 n <= 66, u16 n <= 179, u32 n <= 554, u64 n <= 1885,
 *            u128 n <= 6832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static storage for pre‑computed values */
static seq_t *a_tbl = NULL;      /* a[n] values */
static seq_t *dp_tbl = NULL;     /* partition counts for current odd set */
static idx_t tbl_size = 0;       /* current allocated size for both tables */

/* recompute tables up to and including max_n */
static void recompute(idx_t max_n)
{
    size_t len = (size_t)max_n + 1;

    /* free old tables if any */
    if (a_tbl) free(a_tbl);
    if (dp_tbl) free(dp_tbl);

    a_tbl   = (seq_t *)calloc(len, sizeof(seq_t));
    dp_tbl  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!a_tbl || !dp_tbl) {        /* allocation failed – leave as NULL */
        a_tbl   = dp_tbl = NULL;
        tbl_size = 0;
        return;
    }

    /* a(0) = 1 by definition */
    a_tbl[0] = (seq_t)1;
    tbl_size = len;

    /* initial dp[0] = 1 (empty partition) */
    dp_tbl[0] = (seq_t)1;

    for (idx_t m = 1; ; ++m) {
        idx_t odd = 2 * m - 1;              /* next odd part to add */

        /* add the new coin (odd) to the partition table */
        for (idx_t j = odd; j <= max_n; ++j)
            dp_tbl[j] += dp_tbl[j - odd];

        idx_t base = 2 * m * m;             /* mandatory minimal contribution */
        if (base > max_n) break;           /* no more m can contribute */

        /* add contributions to a_tbl for all N = base + t */
        for (idx_t t = 0; t + base <= max_n; ++t)
            a_tbl[base + t] += dp_tbl[t];
    }
}

/* make sure we have tables large enough for at least n */
static void ensure_precomputed(idx_t n)
{
    if (a_tbl && n < tbl_size) return;
    recompute(n);
}

/* user function */
seq_t a(idx_t n)
{
    ensure_precomputed(n);
    return a_tbl ? a_tbl[n] : (seq_t)0;
}
