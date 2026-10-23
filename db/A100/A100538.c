/*
 * A100538  Volume of the 3-dimensional box of sides of length equal to
 *          consecutive Padovan numbers (A000931). These boxes form a spiral
 *          in three dimensions similar to the spiral of Fibonacci boxes in
 *          two dimensions.
 * https://oeis.org/A100538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000931(n) * A000931(n+1) * A000931(n+2) where A000931
 *            (Padovan) is defined by P(1)=P(2)=P(3)=1, P(n)=P(n-2)+P(n-3) for
 *            n≥4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

static seq_t *padovan = NULL;   /* 1‑based Padovan array */
static idx_t padovan_cap = 0;   /* allocated capacity */
static idx_t padovan_size = 0;  /* number of initialized entries */

/* Ensure padovan[1..needed] are computed. */
static void ensure_padovan(idx_t needed)
{
    /* grow allocation if necessary */
    if (padovan_cap <= needed) {
        idx_t newcap = padovan_cap ? padovan_cap * 2 : 16;
        if (newcap <= needed) newcap = needed + 1;
        padovan = seq_realloc(padovan, newcap);
        padovan_cap = newcap;
    }
    /* initialise first three Padovan numbers on first use */
    if (padovan_size == 0) {
        if (padovan_cap < 4) {
            idx_t newcap = 4;
            padovan = seq_realloc(padovan, newcap);
            padovan_cap = newcap;
        }
        padovan[1] = (seq_t)1;
        padovan[2] = (seq_t)1;
        padovan[3] = (seq_t)1;
        padovan_size = 3;
    }
    /* compute further Padovan numbers up to 'needed' */
    for (idx_t i = padovan_size + 1; i <= needed; ++i) {
        padovan[i] = padovan[i - 2] + padovan[i - 3];
    }
    padovan_size = needed;
}

seq_t a(idx_t n)
{
    /* n is 1‑based as per OEIS offset. Need P(n), P(n+1), P(n+2). */
    ensure_padovan(n + 2);
    seq_t res = padovan[n];
    res *= padovan[n + 1];
    res *= padovan[n + 2];
    return res;
}
