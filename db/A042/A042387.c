/*
 * A042387  Denominators of continued fraction convergents to sqrt(720).
 * https://oeis.org/A042387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042387: Denominators of continued‑fraction convergents to sqrt(720)
   Recurrence: q[0] = 1; q[1] = 1;
   For n ≥ 2: q[n] = a[n]·q[n‑1] + q[n‑2]
   where a[0] = 26 and for n ≥ 1 the coefficients
   a[1],a[2],a[3],a[4] repeat 1,4,1,52.  */
#include <stdlib.h>

static seq_t *den = NULL;          /* cached denominators                          */
static size_t den_cap = 0;         /* allocated capacity (bytes of seq_t)          */
static size_t den_len = 0;         /* number of terms actually computed           */

seq_t a(idx_t n)
{
    static const int period[4] = {1, 4, 1, 52};
    /* Ensure the cache can hold entries up to n. */
    size_t needed = (size_t)(n + 1);
    if (needed > den_cap) {
        seq_t *tmp = (seq_t *)realloc(den, needed * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory: terminate the program */
        den = tmp;
        den_cap = needed;
    }

    /* Initialise the very first term if required. */
    if (den_len == 0) {
        den[0] = (seq_t)1;        /* q0 = 1 */
        den_len = 1;
    }

    /* Compute missing terms up to n (inclusive). */
    for (size_t i = den_len; i <= (size_t)n; ++i) {
        seq_t a_val;
        if (i == 0) {
            a_val = (seq_t)26;    /* a[0] is not used in the recurrence,
                                    but defined for completeness. */
        } else {
            a_val = (seq_t)period[(i - 1) % 4];
        }

        if (i == 1) {
            /* q1 = a[1]*q0 + 0 */
            den[i] = a_val * den[i - 1];
        } else {
            den[i] = a_val * den[i - 1] + den[i - 2];
        }
    }

    den_len = (size_t)(n + 1);     /* at least up to n now computed   */
    return den[(size_t)n];
}
