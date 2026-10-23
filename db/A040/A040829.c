/*
 * A040829  Continued fraction for sqrt(859).
 * https://oeis.org/A040829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  a(n) = coefficient a_n of the simple continued fraction expansion of sqrt(859)
 *  a(0) is floor(sqrt(859)) = 29.  For n > 0 the remaining terms repeat with
 *  the period [3,4,5,1,1,1,2,2,3,2,19,9,1,2,1,1,4,1,3,11,2,6] and then a(23)=29.
 *  The period is detected by the standard continued‑fraction algorithm.
 */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *period_terms = NULL;
static size_t  period_len  = 0;
static seq_t   a0;

static void init_once(void) {
    if (period_terms != NULL) return;

    /* constant D = 859 */
    const seq_t D = (seq_t)859;

    /* a0 = floor(sqrt(D)) */
    a0 = floor_sqrt(D);

    /* continued‑fraction initialization */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* reserve an initial capacity */
    size_t capacity = 64;
    period_terms = (seq_t *)malloc(sizeof(seq_t) * capacity);
    size_t term_count = 0;

    while (1) {
        /* next step of the continued‑fraction algorithm */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (term_count == capacity) {
            capacity *= 2;
            period_terms = (seq_t *)realloc(period_terms, sizeof(seq_t) * capacity);
        }
        period_terms[term_count++] = a;

        /* period ends when a == 2*a0 */
        if (a == a0 * 2) break;
    }
    period_len = term_count;
}

seq_t a(idx_t n) {
    init_once();
    if (n == 0) return a0;
    idx_t idx = (n - 1) % period_len;
    return period_terms[idx];
}
