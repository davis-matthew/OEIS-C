/*
 * A041190  Numerators of continued fraction convergents to sqrt(106).
 * https://oeis.org/A041190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041190: Numerators of continued‑fraction convergents to sqrt(106).
    Recurrence: a(0) = 10,  a(1) = 31
                 a(n) = p_n * a(n‑1) + a(n‑2),   n ≥ 2
    where p_n for n ≥ 1 is periodic 3,2,1,1,1,1,2,3,20 (period 9).
*/

#include <stdlib.h>
#ifdef __cplusplus
#include "../utils/seq_realloc.h"
#endif

static const int period[9] = {3, 2, 1, 1, 1, 1, 2, 3, 20};

static seq_t *conv      = NULL;   /* array of computed numerators   */
static size_t conv_cap  = 0;       /* number of elements currently allocated */
static size_t conv_len  = 0;       /* number of terms that are already computed */

/* Resize internal array so that it can hold indices 0 … need‑1. */
static void ensure_capacity(size_t need)
{
    if (conv_cap >= need) return;

    size_t new_cap = conv_cap ? conv_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;

#ifdef __cplusplus
    conv = seq_realloc(conv, conv_cap, new_cap);
#else
    conv = (seq_t *)realloc(conv, new_cap * sizeof(seq_t));
#endif
    conv_cap = new_cap;
}

/* Compute a(k) for all k ≤ n and store them.   */
static void compute_up_to(idx_t n)
{
    if ((size_t)n < conv_len) return;   /* already available */

    if (conv_len == 0) {                /* initialise first two terms */
        ensure_capacity(2);
        conv[0] = (seq_t)10;
        conv[1] = (seq_t)31;
        conv_len = 2;
        if ((size_t)n < conv_len) return;
    }

    ensure_capacity((size_t)n + 1);

    for (idx_t i = (idx_t)conv_len; i <= n; ++i) {
        int b = period[(size_t)(i - 1) % 9];
        conv[i] = conv[i - 1] * (seq_t)b + conv[i - 2];
    }

    conv_len = (size_t)n + 1;
}

/* Public function required by the testing harness. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return conv[n];
}
