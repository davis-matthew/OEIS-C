/*
 * A041711  Denominators of continued fraction convergents to sqrt(375).
 * https://oeis.org/A041711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(375).
 *            Sequence a(n) satisfies: a(0) = 1 , a(1) = 2 a(n) = q_n * a(n‑1)
 *            + a(n‑2) for n ≥ 2 where q_n are the partial quotients of the
 *            continued fraction of sqrt(375): 19, (2,1,2,1,5,1,2,1,2,38)
 *            repeating
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static const idx_t period[10] = {2,1,2,1,5,1,2,1,2,38};

static seq_t *den = NULL;
static idx_t len = 0, cap = 0;

/* Ensure that the cache can hold at least 'need' elements */
static void grow_cache(idx_t need)
{
    if (need < cap) return;
    idx_t new_cap = cap ? cap * 2 : 4;
    while (new_cap <= need) new_cap *= 2;
    seq_t *p = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
    if (!p) abort();                /* out‑of‑memory */
    den = p;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialisation */
    if (!den) {
        grow_cache(1);
        den[0] = (seq_t)1;          /* a(0) = 1 */
        len = 1;
    }

    if (n + 1 > len) {
        /* Prepare variables for recurrence */
        seq_t prevprev;              /* a(n-2) */
        seq_t prev;                  /* a(n-1) */
        if (len == 1) {
            prevprev = (seq_t)0;     /* a(-1) = 0 */
            prev = den[0];           /* a(0)  = 1 */
        } else {
            prevprev = den[len - 2];
            prev = den[len - 1];
        }

        /* Compute missing terms up to n */
        for (idx_t k = len; k <= n; ++k) {
            idx_t q;
            if (k == 0) {
                q = 19;                /* the integer part a0 */
            } else {
                q = period[(k - 1) % 10];
            }
            seq_t cur = ((seq_t)q) * prev + prevprev;
            grow_cache(k + 1);
            den[k] = cur;
            prevprev = prev;
            prev = cur;
        }
        len = n + 1;
    }

    return den[n];
}
