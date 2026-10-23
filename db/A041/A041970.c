/*
 * A041970  Numerators of continued fraction convergents to sqrt(508).
 * https://oeis.org/A041970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 21, u64 n <= 41, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041970 – Numerators of continued‑fraction convergents to sqrt(508)
 *
 *  Initial terms (by the standard continued‑fraction expansion of sqrt(508)):
 *    a(0) = 22
 *    a(1) = 23
 *    a(2) = 45
 *    a(3) = 248
 *    ...
 *
 *  For n > 63 the sequence satisfies the linear recurrence
 *    a(n) = 89515213717502*a(n‑32) – a(n‑64)
 *  with the above 64 initial values.  This code generates the first
 *  64 values by the continued‑fraction algorithm and then applies the
 *  recurrence for larger n.  Values are memoised in a static cache so that
 *  calls in any order are handled in O(1) time after the needed terms
 *  have been generated.
 *
 *  The implementation only uses integer arithmetic and no floating‑point
 *  operations.  It is valid C11 and C++17 and respects the constraints
 *  on seq_t and idx_t.  The helper `floor_sqrt` is taken from the
 *  provided utils library.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Cached sequence values --------------------------------------------------- */
static seq_t *cache   = NULL;   /* pointer to the array of terms          */
static size_t cache_len = 0;    /* number of terms stored so far          */
static size_t cache_cap = 0;    /* allocated capacity of the array        */

/* Constant used in the recurrence ---------------------------------------- */
static const seq_t COEFF = (seq_t)89515213717502ULL;   /* 89515213717502 */

/* Continued‑fraction state for generating the first 64 numerators -------- */
static seq_t cf_m      = 0;   /* m_k   */
static seq_t cf_d      = 1;   /* d_k   */
static seq_t cf_a0     = 0;   /* floor(sqrt(508)) */
static seq_t cf_a_prev = 0;   /* a_(k‑1)   */
static seq_t p_minus2  = 0;   /* p_(k‑2)   */
static seq_t p_minus1  = 0;   /* p_(k‑1)   */

/* -------------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    /* Grow the cache array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we reach n */
    const idx_t D = 508;
    seq_t sqrtD = floor_sqrt((seq_t)D);   /* floor(sqrt(508)) */

    while (cache_len <= n) {
        if (cache_len == 0) {
            /* a(0) */
            cache[0] = sqrtD;
            cf_a0 = sqrtD;
            cf_a_prev = sqrtD;
            p_minus2 = (seq_t)1;   /* p_(−1) = 1  */
            p_minus1 = sqrtD;      /* p_0    = a0 */
        } else if (cache_len <= 63) {
            /* Next continued‑fraction term (k = cache_len) */
            seq_t m_next = cf_d * cf_a_prev - cf_m;
            seq_t d_next = (D - m_next * m_next) / cf_d;      /* <‑ corrected */
            seq_t a_next = (cf_a0 + m_next) / d_next;

            seq_t numerator = a_next * p_minus1 + p_minus2;
            cache[cache_len] = numerator;

            /* Advance the CF and convergent state */
            cf_m      = m_next;
            cf_d      = d_next;
            cf_a_prev = a_next;

            p_minus2 = p_minus1;
            p_minus1 = numerator;
        } else {
            /* Recurrence for n > 63 */
            seq_t val = COEFF * cache[cache_len - 32] - cache[cache_len - 64];
            cache[cache_len] = val;
        }
        ++cache_len;
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
