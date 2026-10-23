/*
 * A155567  Intersection of A002479 and A020669 : N = a^2 + 2b^2 = c^2 + 5d^2
 *          for some integers a,b,c,d.
 * https://oeis.org/A155567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155567 – intersection of a²+2b² and c²+5d²
 * The function returns the n‑th value (offset 1, a(1)=0).
 * Values are generated on demand; the cache is rebuilt when a
 * larger limit is needed.  The algorithm enumerates all pairs
 * (a,b) and (c,d) up to the current limit and keeps the overlapping
 * numbers.  The sequences grow slowly, so the routine satisfies
 * the required time limits for n up to about 10 000. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data for caching the intersection set */
static seq_t *g_intersection = NULL;   /* sorted intersection values   */
static size_t g_inter_count   = 0;      /* number of values stored      */
static size_t g_inter_cap     = 0;      /* capacity of the array        */
static idx_t  g_limit          = 0;     /* current maximum value searched */

/* expand the intersection array when it becomes full */
static void grow_intersection(void)
{
    size_t new_cap = g_inter_cap ? g_inter_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(g_intersection, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    g_intersection = tmp;
    g_inter_cap = new_cap;
}

/* rebuild the intersection list up to at least *new_limit* */
static void rebuild_intersection(idx_t new_limit)
{
    /* free previous data */
    if (g_intersection) {
        free(g_intersection);
        g_intersection = NULL;
        g_inter_cap = 0;
        g_inter_count = 0;
    }

    size_t len = (size_t)new_limit + 1;
    uint8_t *b1 = (uint8_t *)calloc(len, 1);
    uint8_t *b2 = (uint8_t *)calloc(len, 1);
    if (!b1 || !b2) {
        free(b1); free(b2);
        abort();
    }

    /* a^2 + 2b^2 */
    for (idx_t a = 0; ; ++a) {
        idx_t a2 = a * a;
        if (a2 > new_limit) break;
        for (idx_t b = 0; ; ++b) {
            idx_t n = a2 + 2 * (b * b);
            if (n > new_limit) break;
            b1[(size_t)n] = 1;
        }
    }

    /* c^2 + 5d^2 */
    for (idx_t c = 0; ; ++c) {
        idx_t c2 = c * c;
        if (c2 > new_limit) break;
        for (idx_t d = 0; ; ++d) {
            idx_t n = c2 + 5 * (d * d);
            if (n > new_limit) break;
            b2[(size_t)n] = 1;
        }
    }

    /* collect numbers that appear in both representations */
    for (size_t v = 0; v < len; ++v) {
        if (b1[v] && b2[v]) {
            if (g_inter_count == g_inter_cap) grow_intersection();
            g_intersection[g_inter_count++] = (seq_t)v;
        }
    }

    free(b1);
    free(b2);
    g_limit = new_limit;
}

/* ensure that at least *need* terms are available in the cache */
static void ensure_terms(idx_t need)
{
    if (g_inter_count >= (size_t)need) return;

    if (g_limit == 0) g_limit = 50;          /* start with a small limit */

    /* enlarge the limit until we have enough terms */
    while (g_inter_count < (size_t)need) {
        rebuild_intersection(g_limit);
        if (g_inter_count >= (size_t)need) break;
        if (g_limit > SIZE_MAX / 2) g_limit = SIZE_MAX - 1;
        else g_limit <<= 1;
    }
}

/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* offset 1 – undefined for n < 1 */
    ensure_terms(n);
    return g_intersection[n - 1];
}
