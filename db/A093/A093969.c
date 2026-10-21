/*
 * A093969  a(n) = n*Pell(n-2).
 * https://oeis.org/A093969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * Pell(n-2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                      Pell(0)=0, Pell(1)=1, Pell(k)=2*Pell(k-1)+Pell(k-2) */
/* This implementation memoises Pell numbers in a dynamically‑grown
   array. It works for any integer type provided seq_t and idx_t
   satisfy the arithmetic constraints of the build. */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *pell_cache = NULL;   /* cache of Pell numbers */
static idx_t  pell_cache_size = 0; /* number of elements currently cached */

/* Ensure that pell_cache holds Pell[k] for all 0 <= k <= max_k. */
static void ensure_pell(idx_t max_k)
{
    /* Nothing to do if we already have enough. */
    if (pell_cache_size > max_k)
        return;

    /* First allocation or growth. */
    idx_t new_size = max_k + 1; /* need indices 0..max_k inclusive */
    seq_t *tmp = NULL;

    if (pell_cache == NULL) {          /* first time */
        /* Allocate at least two slots to hold P0 and P1. */
        if (new_size < 2)
            new_size = 2;
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp)
            return; /* allocation failed – no safe fallback */
        tmp[0] = 0;
        tmp[1] = 1;              /* base values */
    } else {                       /* grow existing array */
        tmp = (seq_t *)realloc(pell_cache,
                               new_size * sizeof(seq_t));
        if (!tmp)
            return; /* reallocation failed – leave cache unchanged */
        /* preserve existing values; the new slots are uninitialised */
    }

    /* Compute new Pell values if we grew. */
    idx_t start = (pell_cache == NULL) ? 2 : pell_cache_size;
    for (idx_t i = start; i <= max_k; ++i) {
        tmp[i] = tmp[i - 1] * 2 + tmp[i - 2];
    }

    pell_cache = tmp;
    pell_cache_size = new_size;
}

/* Term of sequence A093969. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    idx_t k = n - 2;                /* index of Pell number needed */
    ensure_pell(k);
    seq_t pell = pell_cache[k];     /* Pell(k) */
    return (seq_t)n * pell;         /* n * Pell(n-2) */
}
