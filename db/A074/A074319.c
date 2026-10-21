/*
 * A074319  Product of next n factorials.
 * https://oeis.org/A074319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sf(2n)/sf(n) (sf(k) = ∏_{i=1..k} i!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static workspace for factorials and superfactorials */
static idx_t LIMIT = 0;
static seq_t *FACT = NULL;
static seq_t *SF   = NULL;

/* enlarge workspace up to new_limit and compute any missing values */
static void ensure_limit(idx_t new_limit)
{
    if (new_limit <= LIMIT)
        return;

    /* reallocate arrays */
    seq_t *tmp_fact = (seq_t *)realloc(FACT, (size_t)(new_limit + 1) * sizeof(seq_t));
    seq_t *tmp_sf   = (seq_t *)realloc(SF,   (size_t)(new_limit + 1) * sizeof(seq_t));

    if (!tmp_fact || !tmp_sf)
        abort(); /* allocation failure */

    /* initialize base (only on first allocation) */
    if (!FACT) {
        tmp_fact[0] = (seq_t)1;   /* 0! = 1 */
        tmp_sf[0]   = (seq_t)1;   /* sf(0) = 1 */
    }

    /* compute missing factorials and superfactorials */
    for (idx_t i = LIMIT + 1; i <= new_limit; ++i) {
        tmp_fact[i] = tmp_fact[i - 1] * (seq_t)i;   /* i!   */
        tmp_sf[i]   = tmp_sf[i - 1] * tmp_fact[i];  /* sf(i)*/
    }

    FACT = tmp_fact;
    SF   = tmp_sf;
    LIMIT = new_limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, safety */

    /* we need factorials and superfactorials up to 2·n */
    ensure_limit(2 * n);

    /* return the upper superfactorial: sf(2n) / sf(n) */
    return SF[2 * n] / SF[n];
}
