/*
 * A042122  Numerators of continued fraction convergents to sqrt(586).
 * https://oeis.org/A042122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042122:  Numerators of continued‑fraction convergents to sqrt(586)
   Recurrence:  a(0) = 24,
                 a(1) = 4·a(0) + 1 = 97,
                 a(n) = c(n)·a(n‑1) + a(n‑2)  for n ≥ 2,
   where c(n) are the partial quotients of the continued fraction for
   √586.  The sequence of quotients is periodic with period 23:
   4, 1, 4, 1, 1, 2, 1, 2, 7, 1, 2, 2, 1, 7, 2, 1, 2, 1, 1, 4, 1, 4, 48.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--- Periodic continued‐fraction coefficients --------------------------------*/
static idx_t *period      = NULL;   /* coefficients for n ≥ 1  */
static size_t period_len  = 0;     /* length of the period        */
static bool   period_done = false; /* true once the period is built */

/* Build the period of the continued fraction for sqrt(586). */
static void build_period(void)
{
    if (period_done)
        return;

    const idx_t D  = 586;       /* the square‑free discriminant    */
    const idx_t a0 = 24;        /* floor(sqrt(586))                */
    idx_t m = 0, d = 1, a = a0;
    idx_t *p = NULL;
    size_t len = 0;

    /* Generate coefficients until the terminating 2·a0 appears. */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* Grow the coefficient array. */
        p = (idx_t *)realloc(p, (len + 1) * sizeof(idx_t));
        p[len++] = a;
    } while (a != 2 * a0);   /* last coefficient is 2·a0 */

    period      = p;
    period_len  = len;
    period_done = true;
}

/*--- Cache of computed convergent numerators -------------------------------*/
static seq_t *cache     = NULL;   /* cache[n] = a(n)                     */
static size_t cache_len = 0;      /* number of values stored in cache    */
static size_t cache_cap = 0;      /* current allocated capacity (≥ cache_len) */

/* Ensure cache contains a(n) for all 0 ≤ n ≤ cache_len-1. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                    /* already computed */

    /* Allocate or grow the cache array. */
    size_t need = (size_t)n + 1;   /* convert idx_t to size_t */
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_cap = need;
    } else if (need > cache_cap) {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache_cap = need;
    }

    /* Compute missing values using the recurrence. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        idx_t coeff;
        if (i == 0) {
            cache[0] = (seq_t)24;                     /* a0 = 24           */
        } else if (i == 1) {
            coeff      = period[0];                   /* a1 = 4            */
            cache[1] = (seq_t)coeff * cache[0] + (seq_t)1;
        } else {
            coeff = period[(i - 1) % period_len];
            cache[i] = (seq_t)coeff * cache[i - 1] + cache[i - 2];
        }
    }

    cache_len = need;
}

/*--- public function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    build_period();           /* guarantee period coefficients are ready   */
    ensure_cache(n);          /* compute a(n) if necessary                 */
    return cache[n];
}
