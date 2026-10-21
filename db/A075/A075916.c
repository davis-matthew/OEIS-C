/*
 * A075916  Third column of triangle A075501.
 * https://oeis.org/A075916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (6^n - 8*12^n + 9*18^n)/2
 *  (Rearranged as (9*18^n - 8*12^n + 6^n)/2 to avoid unsigned underflow.)
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment   */

static seq_t *pow6  = NULL;
static seq_t *pow12 = NULL;
static seq_t *pow18 = NULL;
static size_t pow_cap = 0;

/* Ensure that powers 6^k, 12^k, 18^k for k=0..n are cached.  */
static void ensure_powers(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (pow_cap < needed) {
        size_t new_cap = pow_cap ? pow_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        pow6  = (seq_t *)realloc(pow6,  new_cap * sizeof(seq_t));
        pow12 = (seq_t *)realloc(pow12, new_cap * sizeof(seq_t));
        pow18 = (seq_t *)realloc(pow18, new_cap * sizeof(seq_t));
        if (!pow6 || !pow12 || !pow18) abort();

        /* initialise new entries */
        for (size_t i = pow_cap; i < needed; ++i) {
            if (i == 0) {
                pow6[i]  = (seq_t)1;
                pow12[i] = (seq_t)1;
                pow18[i] = (seq_t)1;
            } else {
                pow6[i]  = pow6[i-1]  * (seq_t)6;
                pow12[i] = pow12[i-1] * (seq_t)12;
                pow18[i] = pow18[i-1] * (seq_t)18;
            }
        }
        pow_cap = needed;
    }
}

seq_t a(idx_t n)
{
    ensure_powers(n);
    size_t i = (size_t)n;

    seq_t term1 = pow6[i];
    seq_t term2 = (seq_t)8 * pow12[i];
    seq_t term3 = (seq_t)9 * pow18[i];

    seq_t sum = term3 - term2 + term1;   /* (9*18^n - 8*12^n + 6^n) */
    return sum / (seq_t)2;
}
