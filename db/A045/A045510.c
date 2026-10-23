/*
 * A045510  Factorials having initial digit '2'.
 * https://oeis.org/A045510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(n) is the n‑th factorial whose leading decimal digit is 2.
 * The code scans consecutive factorial indices, accumulating log10(n!) to
 * determine the leading digit.  Whenever the leading digit is 2 we store the
 * index; after reaching n stored indices the corresponding factorial is
 * returned via the supplied big‑integer routine `factorial`. */
#include <stdlib.h>
#include <math.h>
#include "../utils/factorial.h"        /* seq_t factorial(idx_t n); */

static idx_t   *idx_list  = NULL;    /* indices where n! starts with 2 */
static size_t  idx_cap   = 0;        /* capacity of idx_list              */
static size_t  idx_len   = 0;        /* number of stored indices          */
static idx_t   last_scan = 0;        /* largest factorial index processed */
static long double log_sum = 0.0L;   /* cumulative log10(n!) up to last_scan */

/* Ensure the index array has room for at least one more entry. */
static void ensure_idx_cap(void)
{
    if (idx_len >= idx_cap) {
        size_t new_cap = idx_cap ? idx_cap * 2 : 64;
        idx_t *tmp = (idx_t *)realloc(idx_list, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        idx_list = tmp;
        idx_cap = new_cap;
    }
}

/* Append a new factorial index to the list. */
static void add_idx(idx_t val)
{
    ensure_idx_cap();
    idx_list[idx_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offsets < 1 */

    /* Fill the list until we have n entries. */
    while (idx_len < n) {
        ++last_scan;
        log_sum += log10l((long double)last_scan);

        /* Leading digit from the fractional part of log10(n!) */
        long double frac = log_sum - floorl(log_sum);
        long double p    = powl(10.0L, frac);
        int leading = (int)(p + 1e-12L);   /* guard against rounding */

        if (leading == 2)
            add_idx(last_scan);
    }

    idx_t target = idx_list[n - 1];
    return factorial(target);
}
