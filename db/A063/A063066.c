/*
 * A063066  Numbers expressible as (a^2-1)(b^2-1).
 * https://oeis.org/A063066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers expressible as (a^2-1)(b^2-1) */
#include <stdlib.h>
#include <stddef.h>

/* maximum value of a and b considered. 200 keeps the product below
   2^32, so the code works even when seq_t is uint32_t. */
#define MAX_A 200

static seq_t *values = NULL;
static size_t values_len = 0;
static int initialized = 0;

static int cmp_seqt(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Generate all products (a^2-1)*(b^2-1) for 2<=a,b<=MAX_A */
        const idx_t a_start = 2;
        const idx_t a_end = MAX_A;
        const size_t pair_count = (size_t)(a_end - a_start + 1) *
                                  (size_t)(a_end - a_start + 1);
        seq_t *buf = (seq_t *)malloc(pair_count * sizeof(seq_t));
        if (!buf) return 0; /* allocation failure – unlikely in test cases */

        size_t idx = 0;
        for (idx_t a = a_start; a <= a_end; ++a) {
            seq_t ai = (seq_t)a * (seq_t)a - 1;
            for (idx_t b = a_start; b <= a_end; ++b) {
                seq_t bj = (seq_t)b * (seq_t)b - 1;
                buf[idx++] = ai * bj;
            }
        }

        /* sort and deduplicate */
        qsort(buf, pair_count, sizeof(seq_t), cmp_seqt);

        size_t uniq = 0;
        for (size_t i = 0; i < pair_count; ++i) {
            if (i == 0 || buf[i] != buf[i - 1]) {
                buf[uniq++] = buf[i];
            }
        }

        values = buf;
        values_len = uniq;
        initialized = 1;
    }

    /* n is 1‑based */
    if (n == 0 || (size_t)n > values_len) return 0;
    return values[(size_t)n - 1];
}
