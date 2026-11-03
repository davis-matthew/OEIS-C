/*
 * A040829  Continued fraction for sqrt(859).
 * https://oeis.org/A040829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 87 (2026-11-03)
 * Helpers:   ipow_i32, mem
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
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    period_terms = mem_new(seq_t, capacity);
    size_t term_count = 0;

    while (1) {
        /* next step of the continued‑fraction algorithm */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (term_count == capacity) {
            capacity *= 2;
            period_terms = mem_resize(seq_t, period_terms, capacity);
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
