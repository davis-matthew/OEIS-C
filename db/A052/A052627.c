/*
 * A052627  E.g.f. (1-x)/(1-x-x^5).
 * https://oeis.org/A052627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * g(n), with g(0)=1, g(n)=0 for 1≤n≤4, and for n≥5 :
 *            g(n) = g(n-1) + g(n-5) (from the EGF (1‑x)/(1‑x‑x⁵))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies typedefs for seq_t (value type) and idx_t (index type). */
#include <stdlib.h>

/* Utility to ensure an array can hold indices up to n (inclusive). */
static void
ensure_capacity(seq_t **arr, size_t *size, idx_t n)
{
    if (*size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_size * sizeof(seq_t));
        if (!tmp) return;          /* memory allocation failed – return */
        *arr   = tmp;
        *size  = new_size;
    }
}

/* Sequence storage. */
static seq_t *fac      = NULL;   /* fac[i] = i!          */
static size_t fac_size = 0;
static idx_t  fac_max  = (idx_t)(-1);   /* highest i computed for fac */

static seq_t *garr     = NULL;   /* garr[i] = g(i)       */
static size_t g_size   = 0;
static idx_t  g_max    = (idx_t)(-1);   /* highest i computed for g */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ---- factorials ---------------------------------------------- */
    ensure_capacity(&fac, &fac_size, n);

    if (fac_max == (idx_t)(-1)) {   /* first call: initialise factorial 0! */
        fac[0] = (seq_t)1;
        fac_max = 0;
    }
    for (idx_t i = fac_max + 1; i <= n; ++i) {
        fac[i] = fac[i - 1] * (seq_t)i;
    }
    fac_max = n;

    /* ---- g‑coefficients -------------------------------------------- */
    ensure_capacity(&garr, &g_size, n);

    if (g_max == (idx_t)(-1)) {                 /* first call rendering base values */
        for (idx_t i = 0; i <= n && i < 5; ++i) {
            garr[i] = (i == 0) ? (seq_t)1 : (seq_t)0;
        }
        g_max = 0;
    }

    if (g_max < n) {
        /* If we already have some g values but need to extend beyond them */
        for (idx_t i = g_max + 1; i <= n; ++i) {
            if (i < 5) {
                garr[i] = (i == 0) ? (seq_t)1 : (seq_t)0;
            } else {
                garr[i] = garr[i - 1] + garr[i - 5];
            }
        }
        g_max = n;
    } else {
        /* Ensure the base values are correct in case they were not computed
           (happens when the first call had n < 5). */
        for (idx_t i = 0; i <= n && i < 5; ++i) {
            garr[i] = (i == 0) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* ---- return the sequence value -------------------------------- */
    return fac[n] * garr[n];
}
