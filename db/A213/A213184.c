/*
 * A213184  Numbers which may represent a date in "condensed American
 *          notation" MMDDYY.
 * https://oeis.org/A213184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A213184: Numbers that may represent a date in condensed American notation MMDDYY.
   Each value is of the form m*10000 + d*100 + yy, with:
   - m = 1..12   (month, no leading zero)
   - d = 1..days_in_month(m, yy)   (day, two digits)
   - yy = 00..99  (year suffix)
   For February 29th the year suffix must be divisible by 4 (leap year).
   The sequence consists of all such integers sorted in increasing order.
*/
#include <stdlib.h>
#include "../utils/seq_alloc.h"

static seq_t *a_cache = NULL;
static const idx_t a_count = 36525;   /* 366*25 + 365*75 */

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void init_cache(void)
{
    if (a_cache != NULL) return;

    /* Allocate space for all terms */
    a_cache = seq_alloc(a_count);
    if (!a_cache) return;   /* allocation failed: callers will crash */

    /* Days per month (non‑leap year) */
    static const idx_t mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    idx_t idx = 0;
    for (idx_t m = 1; m <= 12; ++m) {
        for (idx_t yy = 0; yy <= 99; ++yy) {
            idx_t max_d;
            if (m == 2) {
                /* February: 29 days in leap years */
                max_d = (yy % 4 == 0) ? 29 : 28;
            } else {
                max_d = mdays[m-1];
            }
            for (idx_t d = 1; d <= max_d; ++d) {
                seq_t val = (seq_t)m * (seq_t)10000
                            + (seq_t)d * (seq_t)100
                            + (seq_t)yy;
                a_cache[idx++] = val;
            }
        }
    }

    /* Sort the array */
    qsort(a_cache, (size_t)a_count, sizeof(seq_t), seq_cmp);
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    init_cache();
    /* n is 1‑based; indices beyond the sequence size are undefined */
    return a_cache[(size_t)(n - 1)];
}
